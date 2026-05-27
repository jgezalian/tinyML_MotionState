import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

csv = "./drive.csv"

df = pd.read_csv(csv)

time_col = "timestamp"
accel_col = ["a_x", "a_y", "a_z"]
gyro_col = ["dps_x", "dps_y", "dps_z"]

df["time_sec"] = (df[time_col] - df[time_col].iloc[0]) / 1000

df["accel_mag"] = np.sqrt(
    (df[accel_col[0]] ** 2) + (df[accel_col[1]] ** 2) + (df[accel_col[2]] ** 2)
)

df["gyro_mag"] = np.sqrt(
    (df[gyro_col[0]] ** 2) + (df[gyro_col[1]] ** 2) + (df[gyro_col[2]] ** 2)
)

df["minute"] = df["time_sec"] // 60

minute_dfs = {
    minute: group.copy()
    for minute, group in df.groupby("minute")
}

# Accel magnitude
# fig = plt.figure(figsize=(14, 5))
# plt.plot(minute_dfs[0]["time_sec"], minute_dfs[0]["accel_mag"])
# plt.xlabel("time_sec")
# plt.ylabel("accel_mag")
# plt.title("accel")
# fig.canvas.manager.set_window_title('accel')
# plt.grid = True
# plt.show()

# # gyro magnitude
# fig2 = plt.figure(figsize=(14, 5))
# plt.plot(df["time_sec"], df["gyro_mag"])
# plt.xlabel("time_sec")
# plt.ylabel("gyro_mag")
# plt.title("gyro")
# fig2.canvas.manager.set_window_title('gyro')
# plt.grid = True
# plt.show()

#accel axes
# fig3 = plt.figure(figsize=(14,5))
# plt.plot(minute_dfs[1]["time_sec"], minute_dfs[1][accel_col[1]])
# plt.xlabel("time_sec")
# plt.ylabel("accel_x")
# plt.title("accel_x")
# fig3.canvas.manager.set_window_title('accel_x')
# plt.grid = True
# plt.show()

minute_dfs[2]["a_x_smooth"] = minute_dfs[2]["a_x"].rolling(10).mean()

plt.figure(figsize=(14, 5))
plt.plot(minute_dfs[2]["time_sec"], minute_dfs[2]["a_x"], alpha=0.4, label="raw a_x")
plt.plot(minute_dfs[2]["time_sec"], minute_dfs[2]["a_x_smooth"], label="smoothed a_x")
plt.xlabel("Time (s)")
plt.ylabel("X acceleration (g)")
plt.legend()
plt.grid(True)
plt.show()