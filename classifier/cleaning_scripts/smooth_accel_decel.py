import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy.signal import find_peaks

# hard_accel_decel
csv = "../drive_data/first_and_second_combined_raw/smooth_accel_decel/smooth_accel_decel.csv"
df = pd.read_csv(csv)

df["time_sec"] = (df["timestamp"] - df.iloc[0]["timestamp"]) / 1000

attributes = ["a_x", "a_y", "a_z", "dps_x", "dps_y", "dps_z"]

df["a_x_smooth"] = df["a_x"].rolling(window=5, min_periods=1, center=True).mean()

fig = plt.figure(figsize=(14, 5))
plt.plot(df["time_sec"], df["a_x_smooth"])
plt.grid(True)
plt.show()

decel_peaks = find_peaks(df["a_x_smooth"], height=0.05, distance=200)
decel_peak_indices = decel_peaks[0]

accel_peaks = find_peaks(-df["a_x_smooth"], height=0.1, distance=200)
accel_peak_indices = accel_peaks[0]

decel_ranges = []
accel_ranges = []

for peak_index in decel_peak_indices:
    left = peak_index
    right = peak_index
    while df.iloc[left]["a_x_smooth"] >= 0 and left > 0:
        cur = left
        left -= 1
        if (df.iloc[cur]["time_sec"] - df.iloc[left]["time_sec"]) > 0.5:
            left += 1
            break
    while df.iloc[right]["a_x_smooth"] >= 0.00 and right < len(df) - 1:
        cur = right
        if (df.iloc[right]["time_sec"] - df.iloc[cur]["time_sec"]) > 0.5:
            right -= 1
            break
        right += 1
    decel_ranges.append((left, right))

for peak_index in accel_peak_indices:
    left = peak_index
    right = peak_index
    while df.iloc[left]["a_x_smooth"] <= 0.05 and left > 0:
        cur = left
        left -= 1
        if (df.iloc[cur]["time_sec"] - df.iloc[left]["time_sec"]) > 0.5:
            left += 1
            break
    while df.iloc[right]["a_x_smooth"] <= 0.05 and right < len(df) - 1:
        cur = right
        right += 1
        if (df.iloc[right]["time_sec"] - df.iloc[cur]["time_sec"]) > 0.5:
            right -= 1
            break
    accel_ranges.append((left, right))

decel_segments = []
for segment_id, (start, end) in enumerate(decel_ranges):
    segment = df.iloc[start : end + 1].copy()
    segment["segment_time_sec"] = round(
        segment["time_sec"] - segment.iloc[0]["time_sec"], 3
    )
    segment["segment_id"] = segment_id
    decel_segments.append(segment)

decel = pd.concat(decel_segments, ignore_index=True)
decel["target"] = "smooth_decel"
decel = decel.drop(columns=["a_x_smooth"])
decel.to_csv("../drive_data/clean/smooth_accel_decel/decel/decel.csv")

accel_segments = []
for segment_id, (start, end) in enumerate(accel_ranges):
    segment = df.iloc[start : end + 1].copy()
    segment["segment_time_sec"] = round(
        segment["time_sec"] - segment.iloc[0]["time_sec"], 3
    )

    segment["segment_id"] = segment_id
    accel_segments.append(segment)

accel = pd.concat(accel_segments, ignore_index=True)
accel["target"] = "smooth_accel"
accel = accel.drop(columns=["a_x_smooth"])
accel.to_csv("../drive_data/clean/smooth_accel_decel/accel/accel.csv")


def plot_all_attributes(attributes):
    for attribute in attributes:
        fig = plt.figure(figsize=(14, 5))

        for segment_id, segment in decel.groupby("segment_id"):
            plt.plot(segment["segment_time_sec"], segment[attribute])
        plt.xlabel("time_sec")
        plt.ylabel(attribute)
        plt.title(attribute)
        fig.canvas.manager.set_window_title(attribute)
        plt.grid(True)
        plt.savefig(f"../drive_data/clean/smooth_accel_decel/decel/{attribute}.png")

        fig = plt.figure(figsize=(14, 5))
        for segment_id, segment in accel.groupby("segment_id"):
            plt.plot(segment["segment_time_sec"], segment[attribute])
        plt.xlabel("time_sec")
        plt.ylabel(attribute)
        plt.title(attribute)
        fig.canvas.manager.set_window_title(attribute)
        plt.grid(True)
        plt.savefig(f"../drive_data/clean/smooth_accel_decel/accel/{attribute}.png")


plot_all_attributes(attributes)
