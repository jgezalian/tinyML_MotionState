import serial
import time

PORT = "/dev/cu.usbmodem11203"  # replace with your actual port
BAUD = 115200
LABEL = "stationary"

outfile = f"{LABEL}_{int(time.time())}.csv"

with serial.Serial(PORT, BAUD, timeout=1) as ser, open(outfile, "w") as f:
    print(f"Recording to {outfile}. Press Ctrl+C to stop.")

    try:
        while True:
            line = ser.readline().decode(errors="ignore").strip()

            if not line:
                continue

            # Optional: add label column
            if line.startswith("timestamp_ms"):
                f.write(line + ",label\n")
            else:
                f.write(line + f",{LABEL}\n")

            print(line)

    except KeyboardInterrupt:
        print("Stopped.")