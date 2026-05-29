import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

#hard_accel_decel
csv = "../drive_data/raw/hard_accel_decel/filtered_dps_z_hard_accel_decel.csv"
df = pd.read_csv(csv)

df["time_sec_segment"] = (df["time_sec"] - df.groupby("segment_id")["time_sec"].transform(lambda s : s.iloc[0])).round(3)
df.to_csv("../drive_data/clean/hard_accel_decel/hard_accel_decel.csv", index=False)

fig = plt.figure(figsize=(20,10))
for segment_id, segment in df.groupby("segment_id"):
    plt.plot(segment["time_sec_segment"], segment["a_y"])
plt.xlabel("time_sec")
plt.ylabel("a_y")
plt.title("hard_accel_decel_a_y")
fig.canvas.manager.set_window_title('hard_accel_decel_a_y')
plt.xticks(np.arange(0, df["time_sec"].max() + 1, 10), rotation=45)
plt.grid(True)
plt.show()