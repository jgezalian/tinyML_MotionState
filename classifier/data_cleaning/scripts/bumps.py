import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy.signal import find_peaks


def plot_all_attributes(attributes, df, train_data_clean_path):
    for attribute in attributes:
        fig = plt.figure()
        fig.canvas.manager.set_window_title(attribute)
        for bump_id, segment in df.groupby("segment_id"):
            plt.plot(segment["segment_time_sec"], segment[attribute])
        plt.xlabel("segment_time_sec")
        plt.ylabel(attribute)
        plt.title(attribute)
        plt.savefig(f"{train_data_clean_path}/{attribute}.png")
        plt.close()


def clean(df, train_data_clean_path):
    time_col = "timestamp"
    df["time_sec"] = (df[time_col] - df[time_col].iloc[0]) / 1000
    df = df.copy()

    peaks = find_peaks(df["a_z"], height=1.15, distance=50)
    peak_indices = peaks[0]

    ranges = [(idx - 15, idx + 15) for idx in peak_indices]

    segments = []

    for bump_id, (start, end) in enumerate(ranges):
        segment = df.iloc[start : end + 1].copy()

        segment["segment_id"] = bump_id
        segment["segment_time_sec"] = round(
            segment["time_sec"] - segment["time_sec"].iloc[0], 3
        )

        segments.append(segment)

    new_df = pd.concat(segments, ignore_index=True)

    new_df.to_csv(f"{train_data_clean_path}/bumps.csv")

    attributes = ["a_x", "a_y", "a_z", "dps_x", "dps_y", "dps_z"]

    plot_all_attributes(attributes, new_df, train_data_clean_path)
