import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy.signal import find_peaks

csv = "../drive_data/raw/left_turn/left_turn.csv"
df = pd.read_csv(csv)

df["time_sec"] = (df["timestamp"] - df.iloc[0]["timestamp"]) / 1000
peaks = find_peaks(df["dps_z"], height=10, distance=50)
peak_indices = peaks[0]


ranges = []
for peak_index in peak_indices:

    # left valley
    going_up = 0
    cur = peak_index
    while going_up < 5 and cur > 0:
        left = cur - 1
        if df.iloc[left]["dps_z"] > df.iloc[cur]["dps_z"]:
            going_up += 1
        else:
            going_up = 0
        cur -= 1
    left_bound = cur

    # right valley
    going_up = 0
    cur = peak_index
    while going_up < 5 and cur < len(df) - 1:
        right = cur + 1
        if df.iloc[right]["dps_z"] > df.iloc[cur]["dps_z"]:
            going_up += 1
        else:
            going_up = 0
        cur += 1
    right_bound = cur
    ranges.append((left_bound, right_bound))

turns = []
for turn_id, (start, end) in enumerate(ranges):
    turn = df.iloc[start : end + 1].copy()
    turn["segment_id"] = turn_id
    turn["segment_time_sec"] = (turn["time_sec"] - turn["time_sec"].iloc[0]).round(3)
    turns.append(turn)

new_df = pd.concat(turns, ignore_index=True)
new_df.to_csv("../drive_data/clean/left_turn/left_turn.csv")

attributes = ["a_x", "a_y", "a_z", "dps_x", "dps_y", "dps_z"]


def plot_all_attributes(attributes):
    for attribute in attributes:
        fig = plt.figure(figsize=(20, 10))
        for turn_id, turn in new_df.groupby("segment_id"):
            plt.plot(turn["segment_time_sec"], turn[attribute])
        plt.xlabel("time_sec")
        plt.ylabel(attribute)
        plt.title(attribute)
        fig.canvas.manager.set_window_title(attribute)
        plt.grid(True)
        plt.savefig(f"../drive_data/clean/left_turn/{attribute}.png")

plot_all_attributes(attributes)
