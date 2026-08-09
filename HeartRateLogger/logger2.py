#!/usr/bin/env python3

import serial
import csv
import time
import sys
import re
from pathlib import Path
from collections import deque

# Configuration
SERIAL_PORT = "/dev/ttyACM0"
BAUDRATE = 115200
BASE_FILENAME = "serial_log"      # Produces serial_log1.csv, serial_log2.csv, ...

STATUS_WINDOW = 5.0      # seconds for sample-rate calculation
FLUSH_INTERVAL = 1.0     # seconds


def next_filename(base):
    """Return the next available filename: base1.csv, base2.csv, ..."""
    pattern = re.compile(rf"^{re.escape(base)}(\d+)\.csv$")
    indices = []

    for f in Path(".").glob(f"{base}*.csv"):
        m = pattern.match(f.name)
        if m:
            indices.append(int(m.group(1)))

    next_index = max(indices, default=0) + 1
    return f"{base}{next_index}.csv"


CSV_FILENAME = next_filename(BASE_FILENAME)


def format_duration(seconds):
    h = int(seconds // 3600)
    m = int((seconds % 3600) // 60)
    s = int(seconds % 60)
    return f"{h:02d}:{m:02d}:{s:02d}"


def main():
    print(f"Opening {SERIAL_PORT} at {BAUDRATE} baud...", end="", flush=True)

    try:
        with serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1) as ser, \
             open(CSV_FILENAME, "w", newline="", buffering=1) as csvfile:

            print(" CONNECTED")
            print(f"Logging to: {CSV_FILENAME}")

            writer = csv.writer(csvfile)
            writer.writerow(["HostTime_ns", "Data"])

            start_time = time.time()
            last_flush = start_time
            last_status = start_time

            sample_times = deque()
            total_samples = 0
            last_rx_time = start_time

            while True:
                line = ser.readline().decode("ascii", errors="replace").strip()
                now = time.time()

                if line:
                    writer.writerow([time.time_ns(), line])

                    total_samples += 1
                    last_rx_time = now
                    sample_times.append(now)

                    cutoff = now - STATUS_WINDOW
                    while sample_times and sample_times[0] < cutoff:
                        sample_times.popleft()

                if now - last_flush >= FLUSH_INTERVAL:
                    csvfile.flush()
                    last_flush = now

                if now - last_status >= 0.5:
                    if len(sample_times) >= 2:
                        span = sample_times[-1] - sample_times[0]
                        rate = (len(sample_times) - 1) / span if span > 0 else 0.0
                    else:
                        rate = 0.0

                    rx_age = now - last_rx_time
                    status = "RECEIVING" if rx_age < 1.0 else "NO DATA"

                    elapsed = now - start_time

                    sys.stdout.write(
                        "\r"
                        f"[{status:<9}] "
                        f"Elapsed: {format_duration(elapsed)} | "
                        f"Samples: {total_samples:8d} | "
                        f"Rate(5s): {rate:7.2f} Hz | "
                        f"Last RX: {rx_age:4.1f} s ago"
                    )
                    sys.stdout.flush()
                    last_status = now

    except KeyboardInterrupt:
        print("\nLogging stopped by user.")
    except Exception as e:
        print(f"\nError: {e}")


if __name__ == "__main__":
    main()
