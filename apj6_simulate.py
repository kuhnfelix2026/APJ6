#!/usr/bin/python3
"""
Simulation of the APJ6 Carrera car with the controller from APJ6Control.slx.

Run:   python apj6_simulate.py                      (speed steps, cruise control only)
       python apj6_simulate.py --scenario follow    (ACC following a car ahead)
       python apj6_simulate.py --help               (all options)

Writes the result to logs/sim_XX.csv (same columns as the live logs, plus a few
simulation-only columns) and shows a plot if matplotlib is installed.

WHAT IS SIMULATED
-----------------
Car (fitted to logs/live_00.csv, valid above a speed of about 300):
    v[k+1] = 0.9201 * v[k] + 0.897 * (u[k-2] + load[k]) - 22.3
    = first order, gain 11.2 speed per power, time constant 0.60 s,
      100 ms delay, about 25 power needed before the car moves.
    u is the motor power (0..150), load is the extra drag in the corners
    (in power units, so -10 means the corner costs 10 power).

Track: 4 identical straight + curve sections, about 3000 long per lap
    (lap length from the repeating distance-sensor readings in live_00.csv,
    corner spacing from the speed wobble, which repeats every ~740).

Speed sensor: true speed + noise that grows with speed and zig-zags from one
    sample to the next (calibrated to live_00.csv).

Distance sensor: 1500 when nothing is seen. Sees the car ahead (gap + noise)
    when it is closer than 1300 and not hidden behind a curve. Sees the barrier
    at one corner per lap (and sometimes a second one), as in live_00.csv.

Controller (copy of APJ6Control.slx, 0.05 s steps):
    speed -> 2-sample average filter (optional) -> ACC (acc_logic) ->
    Rate Limiter (+1500 / -5000 per s) -> Sum (target - speed) ->
    PI (parallel, forward Euler, 0..150, clamping, level reset when target <= 0)
    -> floor to integer -> motor.

ASSUMPTIONS (not measured)
--------------------------
- Speed is in mm/s and distance in mm (makes a lap about 3 m).
- Coasting with power 0 follows the same model (never logged at speed).
- Distance sensor noise +-8 mm; the car ahead is hidden while a curve lies
  between the two cars.
- Battery voltage and track power are constant.
"""

import argparse
import csv
import math
import os
import random

TS = 0.05

# ---------------------------------------------------------------- car model
CAR_A = 0.9201          # speed memory per step  (time constant 0.60 s)
CAR_B = 0.897           # speed per power per step
CAR_C = -22.3           # friction / losses per step
CAR_DELAY = 2           # steps between setting power and seeing the speed change
POWER_MAX = 1500

# ---------------------------------------------------------------- track model
STRAIGHT = 400.0        # length of each straight
CURVE = 350.0           # length of each curve
SECTIONS = 4
LAP = SECTIONS * (STRAIGHT + CURVE)          # 3000
CORNER_LOAD = 1.8e-5                         # corner drag (power units) per speed^2, calibrated to live_00.csv
WALL_CORNERS = {0: 0.9, 1: 0.5}              # corner index: chance the sensor sees the barrier
WALL_READING = (510.0, 700.0)                # barrier readings seen in the logs
WALL_LENGTH = 250.0                          # distance over which the barrier is seen

# ---------------------------------------------------------------- sensor models
SPEED_NOISE_PER_SPEED = 0.036   # speed sensor noise sd = 0.036 * speed - 11.5, max 30
SPEED_NOISE_OFFSET = -11.5
SPEED_NOISE_MAX = 30.0
SPEED_NOISE_ZIGZAG = 0.6    # next sample tends to go the other way
DIST_NOISE = 8.0
DIST_RANGE = 1300.0
DIST_NONE = 1500.0
SEE_AROUND = 100.0      # how much curve the sensor can look around (guess)


def in_curve(pos):
    """Return (corner index, distance into the curve) or None on a straight."""
    p = pos % LAP
    sec = int(p // (STRAIGHT + CURVE))
    into = p - sec * (STRAIGHT + CURVE) - STRAIGHT
    if into >= 0:
        return sec, into
    return None


CURVE_SHARE = CURVE / (STRAIGHT + CURVE)


def set_track(straight, curve, sections, see_around):
    global STRAIGHT, CURVE, SECTIONS, LAP, CURVE_SHARE, SEE_AROUND
    STRAIGHT, CURVE, SECTIONS, SEE_AROUND = straight, curve, sections, see_around
    LAP = SECTIONS * (STRAIGHT + CURVE)
    CURVE_SHARE = CURVE / (STRAIGHT + CURVE)


def corner_load(pos, speed):
    """Extra drag in a curve (negative power), smooth sine shape over the curve.
    Zero on average over a lap: the fitted car model already contains the
    average drag, so straights get the matching small positive share."""
    c = in_curve(pos)
    shape = 0.0
    if c is not None:
        shape = 2 * math.sin(math.pi * c[1] / CURVE) ** 2
    return CORNER_LOAD * speed * speed * (CURVE_SHARE - shape)


class Car:
    def __init__(self, speed=0.0, pos=0.0, power=None):
        self.v = speed
        self.pos = pos
        steady = max((speed * (1 - CAR_A) - CAR_C) / CAR_B, 0.0) if speed > 0 else 0.0
        p = steady if power is None else power
        self.power_buf = [p] * (CAR_DELAY + 1)

    def step(self, power):
        self.power_buf.append(power)
        delayed = self.power_buf.pop(0)
        load = corner_load(self.pos, self.v)
        v_new = CAR_A * self.v + CAR_B * (delayed + load) + CAR_C
        self.v = max(v_new, 0.0)
        self.pos += self.v * TS


class SpeedSensor:
    def __init__(self, rng):
        self.rng = rng
        self.prev_e = 0.0

    def read(self, v):
        if v <= 0:
            self.prev_e = 0.0
            return 0.0
        sd = min(max(SPEED_NOISE_PER_SPEED * v + SPEED_NOISE_OFFSET, 2.0), SPEED_NOISE_MAX)
        e = self.rng.gauss(0, sd)
        n = (e - SPEED_NOISE_ZIGZAG * self.prev_e) / math.sqrt(1 + SPEED_NOISE_ZIGZAG ** 2)
        self.prev_e = e
        return max(v + n, 0.0)


class DistanceSensor:
    def __init__(self, rng):
        self.rng = rng
        self.lap_walls = {}

    def _wall(self, pos):
        # barrier ahead of the car at the end of a straight (before a curve)
        p = pos % LAP
        lap = int(pos // LAP)
        sec = int(p // (STRAIGHT + CURVE))
        into_straight = p - sec * (STRAIGHT + CURVE)
        if sec not in WALL_CORNERS or not (STRAIGHT - WALL_LENGTH <= into_straight < STRAIGHT):
            return None
        key = (lap, sec)
        if key not in self.lap_walls:
            self.lap_walls[key] = (self.rng.random() < WALL_CORNERS[sec],
                                   self.rng.uniform(*WALL_READING))
        seen, reading = self.lap_walls[key]
        return reading if seen else None

    def read(self, own, lead):
        readings = []
        if lead is not None:
            gap = lead.pos - own.pos
            if 0 < gap < DIST_RANGE and not self._hidden(own.pos, lead.pos):
                readings.append(gap + self.rng.gauss(0, DIST_NOISE))
        w = self._wall(own.pos)
        if w is not None:
            readings.append(w + self.rng.gauss(0, DIST_NOISE))
        return min(readings) if readings else DIST_NONE

    @staticmethod
    def _hidden(p_own, p_lead):
        """The car ahead is hidden when more than SEE_AROUND of curve lies between the cars."""
        curve_between = 0.0
        p = p_own
        while p < p_lead:
            step = min(10.0, p_lead - p)
            if in_curve(p) is not None:
                curve_between += step
            p += step
        return curve_between > SEE_AROUND


# ---------------------------------------------------------------- controller (copy of the Simulink model)
class AccLogic:
    """Copy of acc_logic in APJ6Control.slx. improved=True adds the time gap and hold."""
    NO_CAR, D_STOP, K_GAP, K_REL, N_SEEN = 1400.0, 120.0, 1.0, 0.3, 3
    D0, H, HOLD_STEPS = 150.0, 0.6, 12

    def __init__(self, improved=False):
        self.improved = improved
        self.d1 = self.d2 = self.dprev = 1500.0
        self.rel_f = 0.0
        self.count = 0
        self.hold = 0
        self.last_vacc = 0.0

    def step(self, set_speed, set_gap, d_raw, v_own):
        d_med = sorted([d_raw, self.d1, self.d2])[1]
        self.d2, self.d1 = self.d1, d_raw
        self.count = min(self.count + 1, 100) if d_med < self.NO_CAR else 0
        seen = self.count >= self.N_SEEN
        if seen and self.count > self.N_SEEN:
            rel = (d_med - self.dprev) / TS
            self.rel_f += self.K_REL * (rel - self.rel_f)
        else:
            self.rel_f = 0.0
        self.dprev = d_med
        gap = set_gap if set_gap > 0 else 400.0
        if self.improved:
            gap = max(set_gap, self.D0 + self.H * v_own)
        v_acc = max(v_own + self.rel_f + self.K_GAP * (d_med - gap), 0.0)
        if seen:
            v_target = min(set_speed, v_acc)
            if d_med < self.D_STOP:
                v_target = 0.0
            self.hold, self.last_vacc = self.HOLD_STEPS, v_acc
        elif self.improved and self.hold > 0:
            self.hold -= 1
            v_target = min(set_speed, self.last_vacc)
        else:
            v_target = set_speed
        return v_target, seen, d_med


class Controller:
    def __init__(self, P, I, use_filter=True, acc_improved=False, rise=1500.0, fall=-5000.0):
        self.P, self.I = P, I
        self.use_filter = use_filter
        self.acc = AccLogic(acc_improved)
        self.rise, self.fall = rise, fall
        self.rl = 0.0
        self.integ = 0.0
        self.prev_speed = 0.0

    def warm_start(self, speed):
        """Start as if already cruising steadily at this speed."""
        self.rl = speed
        self.prev_speed = speed
        self.integ = max((speed * (1 - CAR_A) - CAR_C) / CAR_B, 0.0)

    def step(self, set_speed, set_gap, distance, speed):
        v = 0.5 * speed + 0.5 * self.prev_speed if self.use_filter else speed
        self.prev_speed = speed
        v_target, acc_active, d_filt = self.acc.step(set_speed, set_gap, distance, v)
        # Rate Limiter
        self.rl = min(max(v_target, self.rl + self.fall * TS), self.rl + self.rise * TS)
        target = self.rl
        # PID reset (level): target <= 0
        if target <= 0:
            self.integ = 0.0
        e = target - v
        pre = self.P * e + self.integ
        out = min(max(pre, 0.0), POWER_MAX)
        # clamping anti-windup: stop integrating when saturated and pushing further
        if not ((pre > POWER_MAX and e > 0) or (pre < 0 and e < 0)):
            self.integ += TS * self.I * e
        return out, target, acc_active, d_filt


# ---------------------------------------------------------------- scenarios
def steps_schedule():
    """Speed steps like control.csv: (duration s, set_speed)."""
    return [(3, 1100), (3, 900), (3, 1200), (3, 1000), (3, 1150), (2, 0)]


def follow_schedule(lead_stop=True):
    """Own cruise target and the cruise target of the car ahead.
    With lead_stop the car ahead cuts its power at 24 s and coasts to a stop."""
    own = [(32, 1200)]
    lead = [(8, 1000), (8, 800), (8, 1100), (8, 0 if lead_stop else 1000)]
    return own, lead


def read_schedule(path):
    rows = []
    with open(path, newline="") as f:
        reader = csv.reader(f)
        next(reader)
        for r in reader:
            if r and r[0].strip():
                rows.append((float(r[0]) / 1000.0, float(r[1])))
    return rows


def expand(schedule):
    out = []
    for dur, val in schedule:
        out += [float(val)] * int(round(dur / TS))
    return out


def next_log_name(base="sim"):
    os.makedirs("logs", exist_ok=True)
    for i in range(100):
        name = os.path.join("logs", f"{base}_{i:02}.csv")
        if not os.path.exists(name):
            return name
    return os.path.join("logs", f"{base}_99.csv")


def simulate(args):
    rng = random.Random(args.seed)
    if args.scenario == "follow":
        own_s, lead_s = follow_schedule(not args.no_lead_stop)
        set_speeds = expand(own_s)
        lead_targets = expand(lead_s)
        n = min(len(set_speeds), len(lead_targets))
        own = Car(speed=1000.0, pos=0.0)
        lead = Car(speed=1000.0, pos=args.start_gap)
        lead_ctrl = Controller(args.P, args.I, use_filter=True)
        lead_sensor = SpeedSensor(rng)
    else:
        set_speeds = expand(read_schedule(args.schedule) if args.schedule else steps_schedule())
        n = len(set_speeds)
        own = Car(speed=0.0)
        lead = None
    ctrl = Controller(args.P, args.I, use_filter=not args.no_filter, acc_improved=args.acc_improved)
    if args.scenario == "follow":
        ctrl.warm_start(own.v)
        lead_ctrl.warm_start(lead.v)
    ss = SpeedSensor(rng)
    ds = DistanceSensor(rng)

    rows = []
    for k in range(n):
        speed = ss.read(own.v)
        distance = ds.read(own, lead)
        power, target, acc_active, d_filt = ctrl.step(set_speeds[k], args.gap, distance, speed)
        power_int = math.floor(power)
        row = {
            "car_id": 0, "millis": int(k * TS * 1000), "set_speed": set_speeds[k],
            "set_distance": args.gap, "in_a": 0, "in_b": 0, "in_c": 0, "in_d": 0, "in_e": 0,
            "speed": round(speed, 2), "distance": round(distance, 1),
            "out_a": round(d_filt, 1), "out_b": set_speeds[k], "out_c": round(power, 3),
            "out_d": 0, "out_e": 0,
            "sim_true_speed": round(own.v, 2), "sim_target": round(target, 2),
            "sim_acc_active": int(acc_active),
            "sim_lead_speed": round(lead.v, 2) if lead else "",
            "sim_gap": round(lead.pos - own.pos, 1) if lead else "",
        }
        rows.append(row)
        if lead is not None:
            lp, _, _, _ = lead_ctrl.step(lead_targets[k], 0, DIST_NONE, lead_sensor.read(lead.v))
            lead.step(math.floor(lp))
            if lead.pos - own.pos <= 0:
                print(f"Collision at t = {k * TS:.2f} s")
                own.step(power_int)
                break
        own.step(power_int)
    return rows


def summarize(rows, scenario):
    v = [r["sim_true_speed"] for r in rows]
    p = [r["out_c"] for r in rows]
    print(f"Simulated {len(rows) * TS:.1f} s")
    print(f"  max speed {max(v):.0f}, power at limit {sum(x >= POWER_MAX - 1e-6 for x in p) * TS:.1f} s")
    if scenario == "follow":
        gaps = [r["sim_gap"] for r in rows if r["sim_gap"] != ""]
        print(f"  closest gap {min(gaps):.0f}, final gap {gaps[-1]:.0f}")


def plot(rows, scenario, title):
    try:
        import matplotlib.pyplot as plt
    except ImportError:
        print("matplotlib not installed: no plot (pip install matplotlib). The CSV has all data.")
        return
    t = [r["millis"] / 1000 for r in rows]
    n_ax = 3 if scenario == "follow" else 2
    fig, ax = plt.subplots(n_ax, 1, sharex=True, figsize=(10, 3 * n_ax))
    ax[0].plot(t, [r["set_speed"] for r in rows], label="set_speed", lw=1, ls="--")
    ax[0].plot(t, [r["sim_target"] for r in rows], label="target (after ACC + rate limiter)", lw=1.5)
    ax[0].plot(t, [r["speed"] for r in rows], label="measured speed", lw=0.8, alpha=0.6)
    ax[0].plot(t, [r["sim_true_speed"] for r in rows], label="true speed", lw=1.5)
    if scenario == "follow":
        ax[0].plot(t, [r["sim_lead_speed"] for r in rows], label="car ahead", lw=1.2)
    ax[0].set_ylabel("speed"); ax[0].legend(loc="upper right", fontsize=8); ax[0].grid(alpha=0.3)
    ax[1].plot(t, [r["out_c"] for r in rows], label="PID power", lw=1)
    ax[1].axhline(POWER_MAX, color="gray", lw=0.8, ls=":")
    ax[1].set_ylabel("power"); ax[1].grid(alpha=0.3)
    if scenario == "follow":
        ax[2].plot(t, [r["sim_gap"] for r in rows], label="true gap", lw=1.5)
        ax[2].plot(t, [r["distance"] for r in rows], label="distance sensor", lw=0.8, alpha=0.6)
        ax[2].set_ylim(0, 1600); ax[2].set_ylabel("distance"); ax[2].legend(fontsize=8); ax[2].grid(alpha=0.3)
    ax[-1].set_xlabel("time (s)")
    fig.suptitle(title)
    fig.tight_layout()
    plt.show()


def main():
    ap = argparse.ArgumentParser(description="Simulate the APJ6 car with the APJ6Control.slx controller.")
    ap.add_argument("--scenario", choices=["steps", "follow"], default="steps")
    ap.add_argument("--schedule", help="steps scenario: read targets from a CSV in control.csv format")
    ap.add_argument("--P", type=float, default=0.14)
    ap.add_argument("--I", type=float, default=0.25)
    ap.add_argument("--no-filter", action="store_true", help="no 2-sample average on the speed")
    ap.add_argument("--acc-improved", action="store_true", help="time gap (150 + 0.6 s x speed) and 0.6 s hold")
    ap.add_argument("--gap", type=float, default=0, help="set_distance sent to the car (0 = ACC default)")
    ap.add_argument("--start-gap", type=float, default=700, help="follow scenario: initial gap")
    ap.add_argument("--no-lead-stop", action="store_true",
                    help="follow scenario: the car ahead keeps driving instead of stopping at 24 s")
    ap.add_argument("--seed", type=int, default=1, help="random seed for the sensor noise")
    ap.add_argument("--straight", type=float, default=STRAIGHT, help="length of each straight")
    ap.add_argument("--curve", type=float, default=CURVE, help="length of each curve")
    ap.add_argument("--sections", type=int, default=SECTIONS, help="number of straight+curve sections per lap")
    ap.add_argument("--see-around", type=float, default=SEE_AROUND,
                    help="how much curve may lie between the cars before the sensor loses the car ahead")
    ap.add_argument("--no-plot", action="store_true")
    args = ap.parse_args()
    set_track(args.straight, args.curve, args.sections, args.see_around)

    rows = simulate(args)
    name = next_log_name()
    with open(name, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0].keys()))
        w.writeheader()
        w.writerows(rows)
    print(f"Saved {name}")
    summarize(rows, args.scenario)
    if not args.no_plot:
        title = f"{args.scenario}: P={args.P}, I={args.I}" + ("" if args.no_filter else ", filter") + \
                (", improved ACC" if args.acc_improved else "")
        plot(rows, args.scenario, title)


if __name__ == "__main__":
    main()
