import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

#ctr_clock_loop
csv = "./drive_data/raw/ctr_clock_loop.csv"
df = pd.read_csv(csv)
time_col = "timestamp"
df["time_sec"] = (df[time_col] - df[time_col].iloc[0]) / 1000
df = df[(df["time_sec"] >= 3) & (df["time_sec"] <= 86)]
df.to_csv("drive_data/clean/ctr_clockwise_circle.csv")

fig = plt.figure()
fig.canvas.manager.set_window_title('ctr_clockwise_circle_clean')
plt.plot(df["time_sec"], df["dps_z"])
plt.xlabel("time_sec")
plt.ylabel("dps_z")
plt.title("ctr_clockwise_circle_dps_z")
plt.show()