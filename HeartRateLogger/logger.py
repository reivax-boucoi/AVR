
import serial
import csv
import time

SERIAL_PORT = "/dev/ttyACM0"
BAUDRATE = 115200

with serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1) as ser, \
     open("serial_log.csv", "w", newline="", buffering=1) as csvfile:

    writer = csv.writer(csvfile)
    writer.writerow(["HostTime_ns", "Data"])

    while True:
        line = ser.readline().decode("ascii", errors="replace").strip()
        if line:
            writer.writerow([time.time_ns(), line])
