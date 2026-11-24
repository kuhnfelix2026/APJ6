#!/usr/bin/python3
"""
One-time tidy-up of the CarreraUdpTest folder.

- Moves result logs (control_00.csv, control_01.csv, ..., live_00.csv, ...)
  into the logs/ folder.
- Moves logs that contain only a header line (runs where no data arrived)
  into _to_delete/, so you can check and delete them yourself.
- Never touches control.csv, the scripts, the Simulink model or build files.
- Nothing is deleted, and existing files are never overwritten.

Run:  python tidy_project.py
"""

import os
import re
import shutil

here = os.path.dirname(os.path.abspath(__file__))
os.chdir(here)

LOG_PATTERN = re.compile(r"^(control|live)_\d\d\.csv$")

moved, empty, skipped = [], [], []
for name in sorted(os.listdir(".")):
    if not os.path.isfile(name) or not LOG_PATTERN.match(name):
        continue
    with open(name, newline="") as f:
        data_lines = sum(1 for line in f if line.strip()) - 1  # minus header
    dest_dir = "_to_delete" if data_lines <= 0 else "logs"
    os.makedirs(dest_dir, exist_ok=True)
    dest = os.path.join(dest_dir, name)
    if os.path.exists(dest):
        skipped.append(name)
        continue
    shutil.move(name, dest)
    (empty if dest_dir == "_to_delete" else moved).append(name)

print(f"Moved {len(moved)} logs to logs/")
if empty:
    print(f"Moved {len(empty)} empty logs (header only) to _to_delete/: {', '.join(empty)}")
if skipped:
    print(f"Skipped {len(skipped)} files that already exist at the destination: {', '.join(skipped)}")
if not (moved or empty or skipped):
    print("Nothing to tidy.")
