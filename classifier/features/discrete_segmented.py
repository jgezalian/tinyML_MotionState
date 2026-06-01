import pandas as pd
import numpy as np
from scipy.signal import find_peaks

motions = [
    "bumps",
    "hard_accel_decel",
    "left_turn",
    "left_u_turn",
    "right_turn",
    "right_u_turn",
    "smooth_accel_decel",
]

attribute_list = ["a_x", "a_y", "a_z", "dps_x", "dps_y", "dps_z"]


def range(segment, attribute):
    return segment[attribute].max() - segment[attribute].min()


def abs_max(segment, attribute):
    return max(abs(segment[attribute].min()), segment[attribute].max())


def delta(segment, attribute):
    return segment[attribute].iloc[-1] - segment[attribute].iloc[0]


def max_neighbor_diff(segment, attribute):
    return segment[attribute].diff().max()


def min_neighbor_diff(segment, attribute):
    return segment[attribute].diff().min()


def avg_neighbor_diff(segment, attribute):
    return segment[attribute].diff().mean()


def max_gradient(segment, attribute):
    segment[f"{attribute}_smooth"] = (
        segment[attribute].rolling(window=5, center=True, min_periods=1).mean()
    ).to_numpy()
    gradient = np.gradient(
        segment[f"{attribute}_smooth"].to_numpy(),
        segment["segment_time_sec"].to_numpy(),
    )

    return np.max(gradient)


def min_gradient(segment, attribute):
    segment[f"{attribute}_smooth"] = (
        segment[attribute].rolling(window=5, center=True, min_periods=1).mean()
    ).to_numpy()
    gradient = np.gradient(
        segment[f"{attribute}_smooth"].to_numpy(),
        segment["segment_time_sec"].to_numpy(),
    )

    return np.min(gradient)


def num_peaks(segment, attribute):
    segment[f"{attribute}_smooth"] = (
        segment[attribute].rolling(window=5, center=True, min_periods=1).mean()
    )
    peaks = find_peaks(
        segment[f"{attribute}_smooth"],
        prominence=segment[f"{attribute}_smooth"].std(),
        distance=50,
    )
    return len(peaks[0])


def zero_crossings(segment, attribute, window=5):
    x_smooth = (
        segment[attribute]
        .rolling(window=window, center=True, min_periods=1)
        .mean()
        .to_numpy()
    )

    if attribute.startswith("a_"):
        deadband = 0.05  # g
    elif attribute.startswith("dps_"):
        deadband = 2.0  # deg/sec
    else:
        deadband = 0.0

    signs = np.zeros(len(segment[attribute]), dtype=int)
    signs[segment[attribute] > deadband] = 1
    signs[segment[attribute] < -deadband] = -1

    signs = signs[signs != 0]

    if len(signs) < 2:
        return 0

    return int(np.sum(signs[1:] != signs[:-1]))


def extract_attribute_features(segment, attribute):
    return {
        f"{attribute}_mean": segment[attribute].mean(),
        f"{attribute}_std": segment[attribute].std(),
        f"{attribute}_min": segment[attribute].min(),
        f"{attribute}_max": segment[attribute].max(),
        f"{attribute}_range": range(segment, attribute),
        f"{attribute}_abs_max": abs_max(segment, attribute),
        f"{attribute}_delta": delta(segment, attribute),
        f"{attribute}_max_neighbor_diff": max_neighbor_diff(segment, attribute),
        f"{attribute}_min_neighbor_diff": min_neighbor_diff(segment, attribute),
        f"{attribute}_avg_neighbor_diff": avg_neighbor_diff(segment, attribute),
        f"{attribute}_max_gradient": max_gradient(segment, attribute),
        f"{attribute}_min_gradient": min_gradient(segment, attribute),
        f"{attribute}_num_peaks": num_peaks(segment, attribute),
        f"{attribute}_zero_crossings": zero_crossings(segment, attribute),
    }

def magnitude(segment):
    segment["a_mag"] = np.sqrt(segment["a_x"]**2 + segment["a_y"]**2 + segment["a_z"]**2)
    segment["dps_mag"] = np.sqrt(segment["dps_x"]**2 + segment["dps_y"]**2 + segment["dps_z"]**2)

def extract_segment_features(segment_id, segment, motion):

    magnitude(segment)
    row = {
        "source_dataset": f"clean/{motion}.csv",
        "segment_id": segment_id,
        "label": motion,
        "duration": segment.iloc[-1]["segment_time_sec"]
        - segment.iloc[0]["segment_time_sec"],
        "num_samples": len(segment),
        "accel_mag_mean" : segment["a_mag"].mean(),
        "accel_mag_max" : segment["a_mag"].max(),
        "accel_mag_min" : segment["a_mag"].min(),
        "accel_mag_range": segment["a_mag"].max() - segment["a_mag"].min(),
        "dps_mag_mean" : segment["dps_mag"].mean(),
        "dps_mag_max" : segment["dps_mag"].max(),
        "dps_mag_min" : segment["dps_mag"].min(),
        "dps_mag_range" : segment["dps_mag"].max() - segment["dps_mag"].min()
    }

    for attribute in attribute_list:
        row.update(extract_attribute_features(segment, attribute))

    return row


features_rows = []
for motion in motions:
    df = pd.read_csv(f"../drive_data/clean/{motion}/{motion}.csv")
    for segment_id, segment in df.groupby("segment_id"):
        features_rows.append(extract_segment_features(segment_id, segment, motion))

features_df = pd.DataFrame(features_rows)
features_df.to_csv("discrete.csv")
