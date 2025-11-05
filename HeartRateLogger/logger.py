
import serial
import csv
import time
from datetime import datetime

# Configuration
SERIAL_PORT = '/dev/ttyACM0' 
BAUDRATE = 115200
CSV_FILENAME = 'serial_log.csv'

def main():
    try:
        with serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1) as ser, open(CSV_FILENAME, 'a', newline='') as csvfile:
            writer = csv.writer(csvfile)
            
            # Write header if file is empty
            if csvfile.tell() == 0:
                writer.writerow(['Timestamp', 'Data'])

            print(f"Logging data from {SERIAL_PORT} at {BAUDRATE} baud. Press Ctrl+C to stop.")
            
            while True:
                if ser.in_waiting:
                    line = ser.readline().decode('ascii', errors='replace').strip()
                    if line:
                        timestamp = datetime.now().strftime('%H:%M:%S.%f')[:-3]  # Millisecond precision
                        writer.writerow([timestamp, line])
                        csvfile.flush()  # Ensure data is written immediately
    except KeyboardInterrupt:
        print("\nLogging stopped by user.")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == '__main__':
    main()
