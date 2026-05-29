import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

csv = "./drive_data/raw/smooth_constant_speed/smooth_constant_speed.csv"
df = pd.read_csv(csv)

time_col = "timestamp"
accel_cols = ["a_x", "a_y", "a_z"]
gyro_cols = ["dps_x", "dps_y", "dps_z"]

df["time_sec"] = (df[time_col] - df[time_col].iloc[0]) / 1000

df = df[df["dps_z"].between(-1.5, 1.5)]

df["accel_mag"] = np.sqrt(
    (df[accel_cols[0]] ** 2) + (df[accel_cols[1]] ** 2) + (df[accel_cols[2]] ** 2)
)

df["gyro_mag"] = np.sqrt(
    (df[gyro_cols[0]] ** 2) + (df[gyro_cols[1]] ** 2) + (df[gyro_cols[2]] ** 2)
)

df["minute"] = df["time_sec"] // 60


minute_dfs = {
    minute: group.copy()
    for minute, group in df.groupby("minute")
}



fig = plt.figure(figsize=(14,5))
plt.plot(df["time_sec"], df["a_y"])
plt.xlabel("time_sec")
plt.ylabel("a_y")
plt.title("smooth_constant_speed_a_y")
fig.canvas.manager.set_window_title('smooth_constant_speed_a_y')
plt.xticks(np.arange(0, df["time_sec"].max() + 1, 10), rotation=45)
plt.grid(True)
plt.show()

# fig = plt.figure(figsize=(14,5))
# plt.plot(df["time_sec"], df["a_y"])
# plt.xlabel("time_sec")
# plt.ylabel("a_y")
# plt.title("weave_a_y")
# fig.canvas.manager.set_window_title('weave_a_y')
# plt.grid = True
# plt.show()

# fig1 = plt.figure(figsize=(14,5))
# plt.plot(df["time_sec"], df[gyro_cols[2]])
# plt.xlabel("time_sec")
# plt.ylabel("dps_z")
# plt.title("weave_dps_z")
# fig1.canvas.manager.set_window_title('weave_dps_z')
# plt.grid = True
# plt.show()



