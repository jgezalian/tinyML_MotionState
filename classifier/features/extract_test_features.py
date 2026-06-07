from scripts import continuous_segmented, discrete_segmented
import pandas as pd

discrete_motions = [
    "bumps",
    "hard_accel",
    "hard_decel",
    "left_turn",
    "left_u_turn",
    "right_turn",
    "right_u_turn",
    "smooth_accel",
    "smooth_decel",
]

continuous_motions = [
    "weave",
    "clockwise_circle",
    "ctr_clockwise_circle",
    "constant_speed",
    "stationary"
]

attribute_list = ["a_x", "a_y", "a_z", "dps_x", "dps_y", "dps_z"]

discrete_feature_rows = []
for motion in discrete_motions:

    df = pd.read_csv(f"../drive_data/clean/test/{motion}/{motion}.csv")
    for segment_id, segment in df.groupby("segment_id"):
        discrete_feature_rows.append(
            discrete_segmented.extract_segment_features(segment_id, segment, motion)
        )
discrete_feature_rows = [row for window in discrete_feature_rows for row in window]

continuous_feature_rows = []
for motion in continuous_motions:

    df = pd.read_csv(f"../drive_data/clean/test/{motion}/{motion}.csv")
    segments = continuous_segmented.time_split(df)
    new_df = pd.concat(segments)
    for segment_id, segment in new_df.groupby("segment_id"):
        continuous_feature_rows.append(
            continuous_segmented.extract_segment_features(segment_id, segment, motion)
        )



discrete_df = pd.DataFrame(discrete_feature_rows)
discrete_df.to_csv("test/discrete_features.csv")
continuous_df = pd.DataFrame(continuous_feature_rows)
continuous_df.to_csv("test/continuous_features.csv")

