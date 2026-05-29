import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# clockwise_loop
csv = "./drive_data/raw/clockwise_loop.csv"
df = pd.read_csv(csv)
time_col = "timestamp"
df["time_sec"] = (df[time_col] - df[time_col].iloc[0]) / 1000
df = df[(df["time_sec"] >= 2) & (df["time_sec"] <= 115)]
df.to_csv("drive_data/clean/clockwise_circle.csv")

fig = plt.figure()
fig.canvas.manager.set_window_title('clockwise_circle_clean')
plt.plot(df["time_sec"], df["dps_z"])
plt.xlabel("time_sec")
plt.ylabel("dps_z")
plt.title("clockwise_circle_dps_z")
plt.show()