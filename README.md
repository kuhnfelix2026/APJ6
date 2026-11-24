# APJ6Control: slot car cruise control with adaptive cruise control

A Simulink model runs on a Carrera slot car (Arduino Nano 33 IoT) and controls its speed with a PI controller. An adaptive cruise control (ACC) layer uses the car's distance sensor to slow down for an object ahead. A Python program on the laptop sends the target speed over Wi-Fi (UDP) through a web page with a slider, and logs everything the car sends back.

## Folder layout

| Path | What it is |
|---|---|
| `APJ6Control.slx` | The Simulink model. |
| `APJ6_interface.h`, `apj6_car.a` | Precompiled car library, called from the model's C Caller blocks. |
| `apj6_live_control.py` | Live control: web page with a speed slider, live chart and logging. |
| `logs/` | Logged runs (`live_XX.csv`, `control_XX.csv`). |

## Hardware and car library

- Board: Arduino Nano 33 IoT (SAMD21, Cortex-M0+), set as the hardware board in the model.
- The library `apj6_car.a` contains drivers for Wi-Fi/UDP, an LSM6DS3 motion sensor, a VL53L1X laser distance sensor, a NeoPixel LED and the speed sensor.
- The functions the model can call are listed in `APJ6_interface.h`.
- Wi-Fi network name and password are set in the two String Constant blocks inside the model's Initialize Function. They are passed to `APJ6_Car_initialize`.

## Communication

The car and laptop exchange 40-byte UDP packets on port 2390, format `<IIfffffffI` (little-endian):

| Field | Laptop → car | Car → laptop |
|---|---|---|
| uint32 | 2 | car ID |
| uint32 | packet counter | packet counter |
| float | `set_speed` | measured speed |
| float | `set_distance` | measured distance |
| 5 × float | inputs `a`–`e` | outputs `a`–`e` (see below) |
| uint32 | laptop time (ms) | car time (ms) |

The car sends a packet every model step (50 ms). Speed and distance in the car's packet are filled in by the library.

Observed in tests: if the car receives no packet for about 2 s, the library stops the motor and raises its `reset` signal. The live control program avoids this by answering every packet from the car.

## Simulink model

The model runs every 0.05 s. Signal flow:

1. **`C Caller input`** (`APJ6_Car_receive_signals`) gives `set_speed`, `set_distance`, inputs `a`–`e` and `reset`.
2. **ACC (`MATLAB Function1`, function `acc_logic`)** takes `set_speed`, `set_distance`, the distance (`APJ6_Car_get_distance`) and the measured speed (`APJ6_Car_get_speed`), and outputs the speed target `v_target`.
3. **`Rate Limiter1`** limits how fast the target changes: rising 1500 per second, falling 5000 per second.
4. **`Sum`** forms the speed error for the PID (see Status below).
5. **`Discrete PID Controller`**: parallel form, P = 0.14, I = 0.25, D = 0, forward Euler, sample time 0.05 s. Output limited to 0–150 with clamping anti-windup.
6. **PID reset** (level): active when the rate-limited target is ≤ 0, or when the library's `reset` signal is non-zero.
7. **`Data Type Conversion`** → **`C Caller power`** (`APJ6_Car_set_power`).

### What the car sends back (`APJ6_Car_send_signals`)

| Output | Signal |
|---|---|
| `a` | Filtered distance from the ACC (`d_filt`) |
| `b` | `set_speed` as received |
| `c` | PID output (motor power) |
| `d` | Input `a` from the laptop |
| `e` | Battery voltage (`APJ6_Car_get_batt_voltage`) |

### ACC logic (`acc_logic`)

- Takes the median of the last 3 distance readings.
- Readings of 1400 or more mean "nothing ahead" (the sensor reports 1500 when it sees nothing).
- An object counts as detected after 3 consecutive samples (150 ms).
- Estimates the closing speed from the change in distance, low-pass filtered (factor 0.3).
- Target while following: estimated speed of the object ahead + 1.0 × (distance − desired gap), never above `set_speed`.
- Desired gap = `set_distance`, or 400 if `set_distance` is 0.
- Target 0 (stop) when the filtered distance is below 120.

## Live control

```
python apj6_live_control.py
```

Opens a web page at http://localhost:8000 with:

- a target speed slider (0–1300, steps of 10), preset buttons and a Stop button; arrow keys change the target, Space stops;
- live readouts of speed, the target the car received, motor power, distance and battery voltage;
- a chart of the last 20 s;
- logging of every packet to `logs/live_XX.csv`.

The program sends `set_distance` = 0, so the ACC uses its default gap of 400.

Safety: the target is set to 0 if the web page stops checking in for 2 s, and Ctrl+C sends a stop before the program exits. Only one program can use UDP port 2390 at a time.

## Log files

Columns: `car_id, millis, set_speed, set_distance, in_a…in_e, speed, distance, out_a…out_e`. The `out_` columns are outputs `a`–`e` from the table above.

Logs recorded before the ×10 Gain was removed from the model (all logs up to and including `live_00.csv`) store `set_speed` as target ÷ 10, and their `out_a` and `out_d` columns follow the older wiring (raw distance and input `a`).

## Measured car behaviour

From `logs/live_00.csv` (speed control only, before the ACC was added):

- Steady power needed above a speed of about 600: power ≈ 0.098 × speed + 14.7 (for example 101 at 900, 124 at 1100). At 460 the car needed 73.
- Highest steady speed at the power limit of 150: about 1,300–1,370.
- Response to a power change: about 11 speed units per unit of power, time constant about 0.6 s, plus a measurement delay estimated between 0 and 100 ms.
- Speed varies by about ±30–40 at a constant target, at a rate that rises with speed. This is caused by the corners.
- The distance sensor reports readings of about 430–690 at a fixed spot on each lap with no car ahead, from the track barrier in a corner.

## Status

- P = 0.14 and I = 0.25 were computed from the measured car model and have not yet been tested on the car.
- In the current model, the `Sum` block subtracts the rate-limited target from `set_speed`. The measured speed only goes to the ACC function, not to the `Sum` block.
- The ACC has not been tested on the car. In simulation, a fixed 400 gap was too short for the car to stop behind a car that stops suddenly, because the car can only coast (power ≥ 0). The ACC also loses the car ahead in corners.
