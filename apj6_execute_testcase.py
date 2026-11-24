#!/usr/bin/python3

print("Importing libraries...")

import random
import socket
import struct
import time
import csv
import sys
import os

LOG_DIR = "logs"  # all result logs (control_XX.csv) are written here

print("Imports ready...")

class CSVInputReader:
    def __init__(self, basename, loop=False):
        if basename.lower().endswith(".csv"):
            basename = basename[0:-4]
        self.name = f"{basename}.csv"
        self.delay, self.set_speed, self.set_distance, self.a, self.b, self.c, self.d, self.e = 0, 0, 0, 0, 0, 0, 0, 0
        self.loop = loop
        self.init_commands()

    def init_commands(self):
        csvfile = open(self.name, newline='')
        self.csvreader = csv.reader(csvfile, delimiter=',', quotechar='|')
        self.csv_row = self.csvreader.__next__()

    def next_command(self):
        try:
            self.csv_row = self.csvreader.__next__()
            self.csv_row = [float(x) for x in self.csv_row]
        except ValueError:
            # ignore for now, empty line in csv?
            pass #print("skipping line")
        except StopIteration:
            if self.loop:
                self.init_commands()
                self.csv_row = ""
                while len(self.csv_row) == 0:
                    self.csv_row = self.csvreader.__next__()
                self.csv_row = [float(x) for x in self.csv_row]
            else:
                # test finished
                exit(0)
        # unpack from row
        try:
            self.delay, self.set_speed, self.set_distance, self.a, self.b, self.c, self.d, self.e = self.csv_row
        except ValueError as e:
            pass #print(f"wrong line in csv: {self.csv_row}")

    def __repr__(self):
        return f"CSVInputReader<{self.name}>"

class CSVLogWriter:
    def __init__(self, basename):
        if basename.lower().endswith(".csv"):
            basename = basename[0:-4]
        os.makedirs(LOG_DIR, exist_ok=True)
        basename = os.path.join(LOG_DIR, os.path.basename(basename))
        for i in range(99):
            test_name = f"{basename}_{i:02}.csv"
            if not os.path.exists(test_name):
                # file available
                break
        self.name = test_name
        # generate header
        self.check_csv()

    def check_csv(self):
        if not os.path.exists(self.name):
            with open(self.name, "w") as f:
                f.write("")
        self.log_csv(*"car_id, millis, set_speed, set_distance, in_a, in_b, in_c, in_d, in_e, speed, distance, out_a, out_b, out_c, out_d, out_e".split(", "))

    def log_csv(self, car_id, millis, set_speed, set_distance, in_a, in_b, in_c, in_d, in_e, speed, distance, out_a, out_b, out_c, out_d, out_e):
        with open(self.name, 'a', newline='') as f:
            writer = csv.writer(f)
            writer.writerow([car_id, millis, set_speed, set_distance,in_a, in_b, in_c, in_d, in_e, speed, distance, out_a, out_b, out_c, out_d, out_e])

    def __repr__(self):
        return f"CSVLogWriter<{self.name}>"

class DataPacketizer:
    def __init__(self):
        self.packet_counter = 0

    def pack(self, a, b, c, d, e, delta, set_speed=-1.0, set_distance=-1.0):
        self.packet_counter += 1
        millis = int(delta*1000)
        return struct.pack("<IIfffffffI", 2, self.packet_counter, set_speed, set_distance, a, b, c, d, e, millis)

    def unpack(self, message):
        # decode packet
        return struct.unpack("<IIfffffffI", message)

class StatusDisplay:
    def __init__(self):
        self.screen_height = 4
        self.width = 140
        self.curr_line = 1
        self.lines = [''] * self.screen_height
        self.printLine(1, '#' * self.width)
        self.printLine(2, "# This is my screen")
        self.printLine(3, "# Let us see if clear works")
        self.printLine(4, '#' * self.width)
        import atexit
        atexit.register(self.scrollEnd)

    def scrollEnd(self):
        for i in range(self.screen_height - self.curr_line + 1):
            self.print()

    def moveTop(self):
        if self.curr_line > 2:
            print('\033[%dA' % (self.curr_line - 1), end='')
        self.curr_line = 1

    def clear(self):
        self.moveTop()
        for i in range(self.screen_height):
            self.print('\r' + (' ' * self.width) + '\r')
            self.lines[i] = ''
        self.moveTop()
    
    def update(self):
        self.moveTop()
        for l in self.lines:
            self.print(l)

    def printLine(self, line, msg):
        if len(msg) > self.width:
            msg = msg[0:self.width]
        self.lines[line-1] = msg
        self.update()

    def print(self, *args):
        print(*args, flush=True)
        self.curr_line += 1



def testdisplay():
    disp = StatusDisplay()
    time.sleep(1)
    disp.clear()
    time.sleep(1)
    disp.print("Line 1")
    disp.print("Line 2")
    disp.print("Line 3")
    disp.print("Line 4")
    time.sleep(1)
    disp.clear()
    disp.print()
    disp.print("Hello")
    time.sleep(1)
    disp.clear()
    disp.print()
    disp.print()
    disp.print("The time to start is now")
    disp.print("...")
    time.sleep(1)
    exit(2)

# handle input arguments
loop_csv = False
echo_on_receive = False
input_csv = None
output_csv = None
for arg in sys.argv:
    print("Handling", arg)
    if arg == '--loop':
        loop_csv = True
    elif arg == '--echo':
        echo_on_receive = True
    elif arg.endswith(".py"):
        continue
    elif input_csv is None:
        input_csv = CSVInputReader(arg)
        print("set input csv file:", input_csv)
    elif output_csv is None:
        output_csv = CSVLogWriter(arg)
        print("set output csv file:", output_csv)
    else:
        print("Argument", arg, " not handled in:", sys.argv)
    # first is input csv
if input_csv is None:
    input_csv = CSVInputReader('control')
if output_csv is None:
    output_csv = CSVLogWriter(input_csv.name)
if loop_csv:
    input_csv.loop = loop_csv

# Avoid scrolling data
disp = StatusDisplay()
disp.clear()

# Now setup upd sockets
receive_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
receive_socket.settimeout(1) # make sure we do not block forever and allow keyboard interrupt
receive_socket.bind(('', 2390))
# re-use receive socket
send_socket = receive_socket

dps = DataPacketizer()

disp.printLine(1, "Waiting for cars...")
cars = []
last = time.time()
started_at = time.time()
prevT = 0
while True:
    rand = random.randint(0, 10)
    count = 0
    a = 0
    b = 0
    c = 0
    d = 0
    e = 0
    millis = 0
    try:
        message, address = receive_socket.recvfrom(1024)
        if not address in cars:
            disp.printLine(1, f"Car connected from {address}")
            cars.append(address)
        try:
            # decode packet
            car_id, count, speed, distance, a, b, c, d, e, millis = dps.unpack(message)
            disp.printLine(2 + cars.index(address), f"dT={millis-prevT:3}, millis={millis:8}, speed={speed:8.2f}, distance={distance:8.2f}, a={a:8.2f}, b={b:8.2f}, c={c:8.2f}, d={d:8.2f}, e={e:8.2f}")
            prevT = millis
            output_csv.log_csv(car_id, millis, input_csv.set_speed, input_csv.set_distance, 
                               input_csv.a, input_csv.b, input_csv.c, input_csv.d, input_csv.e, 
                               speed, distance, 
                               a, b, c, d, e)
            if echo_on_receive:
                send_socket.sendto(dps.pack(input_csv.a, input_csv.b, input_csv.c, input_csv.d, input_csv.e, 
                                            time.time()-started_at,
                                            set_speed=input_csv.set_speed, set_distance=input_csv.set_distance), address)
        except struct.error as err:
            disp.printLine(4, f"error {message}: {len(message)}")
        message = message.upper()
        # check if next command to be sent
        dT = int((time.time() - last)*1000)
        if dT > input_csv.delay:
            # send loaded command
            input_csv.next_command()
            disp.printLine(4, f"Last command: {input_csv.csv_row}")
            # With --echo the new command goes out with the next echo (within one
            # 50 ms step). Sending it here as well would put an extra packet in the
            # car's queue at every row change, so the car falls further behind.
            if not echo_on_receive:
                for a in cars:
                    send_socket.sendto(dps.pack(input_csv.a, input_csv.b, input_csv.c, input_csv.d, input_csv.e, 
                                            time.time()-started_at,
                                            set_speed=input_csv.set_speed, set_distance=input_csv.set_distance), a)
            last = time.time()
    except TimeoutError as e:
        pass
    