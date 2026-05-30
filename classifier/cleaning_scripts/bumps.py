import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

csv = "../drive_data/raw/bumps/bumps.csv"
df = pd.read_csv(csv)
time_col = "timestamp"
df["time_sec"] = (df[time_col] - df[time_col].iloc[0]) / 1000
df = df.copy()
indices = df.index[df['a_z'] >= 1.25]
median_indices = []
group = [indices[0]]
for index in indices[1:]:
    if(index - group[-1] < 100):
        group.append(index)
    else:
        median_indices.append(group)
        group = [index]

center_indices = [int(np.floor(np.median(group))) for group in median_indices]
ranges = [(idx - 20, idx + 20) for idx in center_indices]

segments = []

for bump_id, (start, end) in enumerate(ranges):
    segment = df.iloc[start:end + 1].copy()

    segment["bump_id"] = bump_id
    segment["segment_time_sec"] = round(segment["time_sec"] - segment["time_sec"].iloc[0], 3)

    segments.append(segment)

new_df = pd.concat(segments, ignore_index=True)

new_df.to_csv("../drive_data/clean/bumps/bumps.csv")

attributes = ["a_x", "a_y", "a_z", "dps_x", "dps_y", "dps_z"]

def plot_all_attributes(attributes):
    for attribute in attributes:
        fig = plt.figure()
        fig.canvas.manager.set_window_title(attribute)
        for bump_id, segment in new_df.groupby("bump_id"):
            plt.plot(segment["segment_time_sec"], segment[attribute])
        plt.xlabel("segment_time_sec")
        plt.ylabel(attribute)
        plt.title(attribute)
        plt.savefig(f"../drive_data/clean/bumps/{attribute}.png")

plot_all_attributes(attributes)
