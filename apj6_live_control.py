#!/usr/bin/python3
"""
Live speed control for the APJ6 Carrera car, with a web page.

Run:    python apj6_live_control.py
Then open http://localhost:8000 in your browser (it opens automatically).

- Talks to the car on UDP port 2390, exactly like apj6_execute_testcase.py,
  so do not run both at the same time.
- Sends the current target back to the car after every packet it receives
  (like --echo), so the car's 2 s command timeout never triggers.
- Logs every packet to logs/live_XX.csv in the same format as the test logs.
- Safety: if the web page stops talking to this program for STALE_PAGE_S
  seconds (tab closed, laptop asleep), the target is set to 0.
  Ctrl+C also sends a stop before exiting.
"""

import json
import os
import socket
import struct
import threading
import time
import webbrowser
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

UDP_PORT = 2390
HTTP_PORT = 8000
SPEED_SCALE = 1         # set_speed is sent as real speed (the model no longer has the x10 Gain)
MAX_TARGET = 1300       # highest target the slider allows (car tops out around here)
STALE_PAGE_S = 2.0      # stop the car if the page is silent this long
HISTORY_S = 20          # seconds of data kept for the chart
PACKET_FMT = "<IIfffffffI"

lock = threading.Lock()
state = {
    "target": 0.0,            # target speed in real speed units
    "last_page": time.time(),
    "cars": [],
    "last_rx": 0.0,
    "latest": None,
    "history": [],            # [t, target, speed, power, car_target]
    "log_name": "",
}
started_at = time.time()
packet_counter = 0


LOG_DIR = "logs"


def pick_log_name(base="live"):
    os.makedirs(LOG_DIR, exist_ok=True)
    for i in range(100):
        name = os.path.join(LOG_DIR, f"{base}_{i:02}.csv")
        if not os.path.exists(name):
            return name
    return os.path.join(LOG_DIR, f"{base}_99.csv")


def pack_command(target):
    global packet_counter
    packet_counter += 1
    millis = int((time.time() - started_at) * 1000)
    return struct.pack(PACKET_FMT, 2, packet_counter,
                       target / SPEED_SCALE, 0.0,   # set_speed, set_distance
                       0.0, 0.0, 0.0, 0.0, 0.0,     # a..e
                       millis)


def udp_loop(sock, log_name):
    with open(log_name, "w", newline="") as f:
        f.write("car_id,millis,set_speed,set_distance,in_a,in_b,in_c,in_d,in_e,"
                "speed,distance,out_a,out_b,out_c,out_d,out_e\n")
    while True:
        try:
            message, address = sock.recvfrom(1024)
        except (socket.timeout, TimeoutError):
            continue
        except OSError:
            return  # socket closed on exit
        try:
            car_id, count, speed, distance, a, b, c, d, e, millis = struct.unpack(PACKET_FMT, message)
        except struct.error:
            continue

        now = time.time()
        with lock:
            if now - state["last_page"] > STALE_PAGE_S and state["target"] != 0:
                print("Web page went quiet: target set to 0")
                state["target"] = 0.0
            target = state["target"]
            if address not in state["cars"]:
                state["cars"].append(address)
                print(f"Car connected from {address}")
            state["last_rx"] = now
            state["latest"] = {
                "car_id": car_id, "speed": speed, "distance": distance,
                "power": c, "car_target": b * SPEED_SCALE, "battery": e,
            }
            t = round(now - started_at, 2)
            state["history"].append([t, target, round(speed), round(c, 1), b * SPEED_SCALE])
            cutoff = t - HISTORY_S
            while state["history"] and state["history"][0][0] < cutoff:
                state["history"].pop(0)

        # reply with the current target (keeps the car's timeout fed, one packet per packet)
        sock.sendto(pack_command(target), address)

        with open(log_name, "a", newline="") as f:
            f.write(f"{car_id},{millis},{target / SPEED_SCALE},0,0,0,0,0,0,"
                    f"{speed},{distance},{a},{b},{c},{d},{e}\n")


PAGE = r"""<!DOCTYPE html>
<html lang="en"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>APJ6 live speed control</title>
<style>
  :root { --bg:#f7f6f2; --card:#fff; --text:#22221f; --muted:#6b6a64; --line:#e2e0d8;
          --target:#7f77dd; --speed:#1d9e75; --power:#d85a30; --stop:#c0392b; }
  @media (prefers-color-scheme: dark) {
    :root { --bg:#1d1d1b; --card:#262624; --text:#ecebe6; --muted:#a3a19a; --line:#3a3936; } }
  * { box-sizing:border-box; }
  body { margin:0; font-family:system-ui,-apple-system,"Segoe UI",sans-serif; background:var(--bg); color:var(--text); }
  main { max-width:900px; margin:0 auto; padding:24px 16px 40px; }
  h1 { font-size:20px; font-weight:500; margin:0 0 4px; }
  .sub { color:var(--muted); font-size:14px; margin-bottom:20px; }
  .card { background:var(--card); border:1px solid var(--line); border-radius:12px; padding:20px; margin-bottom:16px; }
  .row { display:flex; align-items:center; gap:16px; flex-wrap:wrap; }
  .big { font-size:40px; font-weight:500; font-variant-numeric:tabular-nums; min-width:120px; }
  input[type=range] { flex:1; min-width:220px; height:32px; accent-color:var(--target); }
  button { font:inherit; font-size:15px; padding:9px 14px; border-radius:8px; border:1px solid var(--line);
           background:var(--card); color:var(--text); cursor:pointer; }
  button:hover { border-color:var(--muted); }
  #stop { background:var(--stop); border-color:var(--stop); color:#fff; font-weight:500; padding:12px 28px; }
  .presets { margin-top:14px; gap:8px; }
  .stats { display:grid; grid-template-columns:repeat(auto-fit,minmax(130px,1fr)); gap:12px; }
  .stat .label { font-size:13px; color:var(--muted); }
  .stat .val { font-size:24px; font-weight:500; font-variant-numeric:tabular-nums; }
  .dot { display:inline-block; width:10px; height:10px; border-radius:50%; background:#999; margin-right:6px; }
  .ok { background:var(--speed); } .bad { background:var(--stop); }
  canvas { width:100%; height:280px; display:block; }
  .legend { display:flex; gap:18px; font-size:13px; color:var(--muted); margin-top:8px; }
  .legend span::before { content:""; display:inline-block; width:14px; height:3px; margin-right:6px; vertical-align:middle; background:var(--c); }
  .hint { font-size:13px; color:var(--muted); margin-top:10px; }
</style></head><body><main>
  <h1>APJ6 live speed control</h1>
  <div class="sub"><span id="dot" class="dot"></span><span id="conn">Waiting for car…</span> · logging to <span id="log"></span></div>

  <div class="card">
    <div class="row">
      <div class="big" id="tval">0</div>
      <input id="slider" type="range" min="0" max="__MAX__" step="10" value="0" aria-label="Target speed">
      <button id="stop">Stop</button>
    </div>
    <div class="row presets">
      <button data-v="900">900</button><button data-v="1000">1000</button>
      <button data-v="1100">1100</button><button data-v="1200">1200</button>
    </div>
    <div class="hint">Arrow keys change the target by 10, Space stops the car. Closing this page also stops the car.</div>
  </div>

  <div class="card stats">
    <div class="stat"><div class="label">Measured speed</div><div class="val" id="s_speed">–</div></div>
    <div class="stat"><div class="label">Target the car has</div><div class="val" id="s_ct">–</div></div>
    <div class="stat"><div class="label">Motor power (out_c)</div><div class="val" id="s_pow">–</div></div>
    <div class="stat"><div class="label">Distance</div><div class="val" id="s_dist">–</div></div>
    <div class="stat"><div class="label">Battery</div><div class="val" id="s_bat">–</div></div>
  </div>

  <div class="card">
    <canvas id="chart"></canvas>
    <div class="legend">
      <span style="--c:var(--target)">Target speed</span>
      <span style="--c:var(--speed)">Measured speed</span>
      <span style="--c:var(--power)">Power ×10</span>
    </div>
  </div>
</main>
<script>
const slider = document.getElementById('slider');
const tval = document.getElementById('tval');
let sendTimer = null;

function setTarget(v, immediate) {
  v = Math.max(0, Math.min(__MAX__, Math.round(v / 10) * 10));
  slider.value = v; tval.textContent = v;
  clearTimeout(sendTimer);
  const send = () => fetch('/set', {method:'POST', headers:{'Content-Type':'application/json'},
                                    body: JSON.stringify({target: v})}).catch(()=>{});
  if (immediate) send(); else sendTimer = setTimeout(send, 60);
}
slider.addEventListener('input', () => setTarget(+slider.value));
document.getElementById('stop').addEventListener('click', () => setTarget(0, true));
document.querySelectorAll('[data-v]').forEach(b => b.addEventListener('click', () => setTarget(+b.dataset.v, true)));
document.addEventListener('keydown', e => {
  if (e.code === 'Space') { e.preventDefault(); setTarget(0, true); }
  else if (e.key === 'ArrowRight' || e.key === 'ArrowUp') { e.preventDefault(); setTarget(+slider.value + 10); }
  else if (e.key === 'ArrowLeft' || e.key === 'ArrowDown') { e.preventDefault(); setTarget(+slider.value - 10); }
});

const canvas = document.getElementById('chart');
const ctx = canvas.getContext('2d');
function css(v) { return getComputedStyle(document.documentElement).getPropertyValue(v).trim(); }

function draw(hist) {
  const dpr = window.devicePixelRatio || 1;
  const W = canvas.clientWidth, H = canvas.clientHeight;
  canvas.width = W * dpr; canvas.height = H * dpr; ctx.setTransform(dpr,0,0,dpr,0,0);
  ctx.clearRect(0,0,W,H);
  const L = 44, R = 8, T = 8, B = 22, yMax = 1600, span = __HIST__;
  const tEnd = hist.length ? hist[hist.length-1][0] : span, tStart = tEnd - span;
  const x = t => L + (t - tStart) / span * (W - L - R);
  const y = v => T + (1 - Math.min(v, yMax) / yMax) * (H - T - B);
  ctx.strokeStyle = css('--line'); ctx.fillStyle = css('--muted'); ctx.font = '11px system-ui'; ctx.lineWidth = 1;
  for (let v = 0; v <= yMax; v += 400) {
    ctx.beginPath(); ctx.moveTo(L, y(v)); ctx.lineTo(W - R, y(v)); ctx.stroke();
    ctx.fillText(v, 6, y(v) + 4);
  }
  for (let s = Math.ceil(tStart / 5) * 5; s <= tEnd; s += 5) ctx.fillText(s + 's', x(s) - 8, H - 6);
  const line = (idx, color, scale, width) => {
    ctx.strokeStyle = color; ctx.lineWidth = width; ctx.beginPath();
    hist.forEach((p, i) => { const px = x(p[0]), py = y(p[idx] * scale); i ? ctx.lineTo(px, py) : ctx.moveTo(px, py); });
    ctx.stroke();
  };
  line(3, css('--power'), 10, 1.2);
  line(2, css('--speed'), 1, 1.6);
  line(1, css('--target'), 1, 2.2);
}

async function poll() {
  try {
    const s = await (await fetch('/state')).json();
    document.getElementById('log').textContent = s.log_name;
    const age = s.rx_age;
    const dot = document.getElementById('dot');
    if (age === null) { document.getElementById('conn').textContent = 'Waiting for car…'; dot.className = 'dot'; }
    else if (age < 1) { document.getElementById('conn').textContent = 'Car connected'; dot.className = 'dot ok'; }
    else { document.getElementById('conn').textContent = 'No data from car for ' + age.toFixed(1) + ' s'; dot.className = 'dot bad'; }
    if (s.latest) {
      document.getElementById('s_speed').textContent = Math.round(s.latest.speed);
      document.getElementById('s_ct').textContent = Math.round(s.latest.car_target);
      document.getElementById('s_pow').textContent = s.latest.power.toFixed(0);
      document.getElementById('s_dist').textContent = Math.round(s.latest.distance);
      document.getElementById('s_bat').textContent = s.latest.battery.toFixed(2) + ' V';
    }
    draw(s.history);
  } catch (e) {
    document.getElementById('conn').textContent = 'Lost connection to apj6_live_control.py';
    document.getElementById('dot').className = 'dot bad';
  }
  setTimeout(poll, 200);
}
fetch('/set', {method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify({target: 0})});
poll();
</script></body></html>
""".replace("__MAX__", str(MAX_TARGET)).replace("__HIST__", str(HISTORY_S))


class Handler(BaseHTTPRequestHandler):
    def log_message(self, *args):
        pass  # keep the console quiet

    def _send(self, code, body, ctype):
        data = body.encode()
        self.send_response(code)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(data)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(data)

    def do_GET(self):
        if self.path == "/":
            self._send(200, PAGE, "text/html; charset=utf-8")
        elif self.path == "/state":
            now = time.time()
            with lock:
                state["last_page"] = now
                body = json.dumps({
                    "target": state["target"],
                    "rx_age": (now - state["last_rx"]) if state["last_rx"] else None,
                    "latest": state["latest"],
                    "history": state["history"],
                    "log_name": state["log_name"],
                })
            self._send(200, body, "application/json")
        else:
            self._send(404, "not found", "text/plain")

    def do_POST(self):
        if self.path != "/set":
            self._send(404, "not found", "text/plain")
            return
        try:
            length = int(self.headers.get("Content-Length", 0))
            target = float(json.loads(self.rfile.read(length))["target"])
        except (ValueError, KeyError, json.JSONDecodeError):
            self._send(400, "bad request", "text/plain")
            return
        target = max(0.0, min(float(MAX_TARGET), target))
        with lock:
            state["target"] = target
            state["last_page"] = time.time()
        print(f"Target speed: {target:.0f}")
        self._send(200, "{}", "application/json")


def main():
    log_name = pick_log_name()
    state["log_name"] = log_name

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.settimeout(0.5)
    sock.bind(("", UDP_PORT))
    threading.Thread(target=udp_loop, args=(sock, log_name), daemon=True).start()

    server = ThreadingHTTPServer(("127.0.0.1", HTTP_PORT), Handler)
    url = f"http://localhost:{HTTP_PORT}"
    print(f"Listening for the car on UDP port {UDP_PORT}, logging to {log_name}")
    print(f"Open {url} in your browser. Press Ctrl+C to stop.")
    threading.Thread(target=lambda: (time.sleep(0.5), webbrowser.open(url)), daemon=True).start()
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        print("Stopping the car...")
        with lock:
            state["target"] = 0.0
            cars = list(state["cars"])
        for _ in range(5):
            for addr in cars:
                try:
                    sock.sendto(pack_command(0.0), addr)
                except OSError:
                    pass
            time.sleep(0.05)
        server.server_close()
        sock.close()


if __name__ == "__main__":
    main()
