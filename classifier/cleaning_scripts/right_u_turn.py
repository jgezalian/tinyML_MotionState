import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy.signal import find_peaks

csv = "../drive_data/raw/right_u_turn/right_u_turn.csv"
df = pd.read_csv(csv)

df["time_sec"] = (df["timestamp"] - df.iloc[0]["timestamp"]) / 1000
peaks = find_peaks(-df["dps_z"], height=10, distance=100)
peak_indices = peaks[0]

ranges = []
for peak_index in peak_indices:

    #left plateau
    going_down = 0
    cur = peak_index
    while(going_down < 6 and cur > 0):
        left = cur - 1
        if(df.iloc[left]["dps_z"] < df.iloc[cur]["dps_z"]):
            going_down += 1
        else: going_down = 0
        cur -= 1
    left_bound = cur
    
    
    #right plateau
    going_down = 0
    cur = peak_index
    while(going_down < 6 and cur < len(df) - 1):
        right = cur + 1
        if(df.iloc[right]["dps_z"] < df.iloc[cur]["dps_z"]):
            going_down += 1
        else: going_down = 0
        cur += 1
    right_bound = cur
    ranges.append((left_bound,right_bound))
    
uturns = []
for uturn_id, (start, end) in enumerate(ranges):
    uturn = df.iloc[start : end + 1].copy()
    uturn["segment_id"] = uturn_id
    uturn["segment_time_sec"] = (uturn["time_sec"] - uturn["time_sec"].iloc[0]).round(3)
    uturns.append(uturn)

new_df = pd.concat(uturns, ignore_index=True)
new_df.to_csv("../drive_data/clean/right_u_turn/right_u_turn.csv")

attributes = ["a_x", "a_y", "a_z", "dps_x", "dps_y", "dps_z"]


def plot_all_attributes(attributes):
    for attribute in attributes:
        fig = plt.figure(figsize=(20, 10))
        for uturn_id, uturn in new_df.groupby("segment_id"):
            plt.plot(uturn["segment_time_sec"], uturn[attribute])
        plt.xlabel("time_sec")
        plt.ylabel(attribute)
        plt.title(attribute)
        fig.canvas.manager.set_window_title(attribute)
        plt.grid(True)
        plt.savefig(f"../drive_data/clean/right_u_turn/{attribute}.png")

plot_all_attributes(attributes)

