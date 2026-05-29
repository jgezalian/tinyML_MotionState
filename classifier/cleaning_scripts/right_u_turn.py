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
    
turns = []
for turn_id, (start, end) in enumerate(ranges):
    turn = df.iloc[start : end + 1].copy()
    turn["uturn_id"] = turn_id
    turn["uturn_time_sec"] = (turn["time_sec"] - turn["time_sec"].iloc[0]).round(3)
    turns.append(turn)

new_df = pd.concat(turns, ignore_index=True)
new_df.to_csv("../drive_data/clean/right_u_turn/right_u_turn.csv")

fig = plt.figure()
fig.canvas.manager.set_window_title('right_u_turn_clean')
for turn_id, turn in new_df.groupby("uturn_id"):
    plt.plot(turn["uturn_time_sec"], turn["dps_z"])
plt.xlabel("uturn_time_sec")
plt.ylabel("dps_z")
plt.title("right_u_turn_dps_z")
plt.show()

