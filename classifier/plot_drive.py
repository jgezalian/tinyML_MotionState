import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

csv = "./drive_data/second_set_raw/hard_accel_decel3.csv"
df = pd.read_csv(csv)

time_col = "timestamp"
accel_cols = ["a_x", "a_y", "a_z"]
gyro_cols = ["dps_x", "dps_y", "dps_z"]

df["time_sec"] = (df[time_col] - df[time_col].iloc[0]) / 1000

fig = plt.figure(figsize=(14,5))
plt.plot(df["time_sec"], df["a_x"])
plt.xlabel("time_sec")
plt.ylabel("a_x")
plt.grid(True)
plt.show()




