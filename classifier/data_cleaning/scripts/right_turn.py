import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy.signal import find_peaks


def plot_all_attributes(attributes, df, train_data_clean_path):
    for attribute in attributes:
        fig = plt.figure(figsize=(14, 5))
        for turn_id, turn in df.groupby("segment_id"):
            plt.plot(turn["segment_time_sec"], turn[attribute])
        plt.xlabel("time_sec")
        plt.ylabel(attribute)
        plt.title(attribute)
        fig.canvas.manager.set_window_title(attribute)
        plt.grid(True)
        plt.savefig(f"{train_data_clean_path}/{attribute}.png")
        plt.close()


def clean(df, train_data_clean_path):

    df["time_sec"] = (df["timestamp"] - df.iloc[0]["timestamp"]) / 1000
    df["dps_z_smooth"] = (
        df["dps_z"].rolling(window=40, min_periods=1, center=True).mean()
    )
    peaks = find_peaks(-df["dps_z_smooth"], height=10, distance=50)
    peak_indices = peaks[0]

    ranges = []
    for peak_index in peak_indices:

        # left plateau
        cur = peak_index
        max = df.iloc[peak_index]["dps_z_smooth"]
        while (df.iloc[cur]["dps_z_smooth"] < 0) and cur > 0:
            cur -= 1
            if df.iloc[cur]["dps_z_smooth"] < max:
                break
            max = df.iloc[cur]["dps_z_smooth"]
        left_bound = cur

        # right plateau
        cur = peak_index
        max = df.iloc[peak_index]["dps_z_smooth"]
        while (df.iloc[cur]["dps_z"] < 1) and cur < len(df) - 1:
            cur += 1
            if df.iloc[cur]["dps_z_smooth"] < max:
                break
            max = df.iloc[cur]["dps_z_smooth"]
        right_bound = cur

        ranges.append((left_bound, right_bound))

    turns = []
    for turn_id, (start, end) in enumerate(ranges):
        turn = df.iloc[start : end + 1].copy()
        turn["segment_id"] = turn_id
        turn["segment_time_sec"] = (turn["time_sec"] - turn["time_sec"].iloc[0]).round(
            3
        )
        turns.append(turn)

    new_df = pd.concat(turns, ignore_index=True)
    new_df = new_df.drop(columns=["dps_z_smooth"])
    new_df.to_csv(f"{train_data_clean_path}/right_turn.csv")

    attributes = ["a_x", "a_y", "a_z", "dps_x", "dps_y", "dps_z"]

    plot_all_attributes(attributes, new_df, train_data_clean_path)
