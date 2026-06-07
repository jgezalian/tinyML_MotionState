from scripts import *
import pandas as pd

PIPELINE = {
    "bumps": bumps,
    "clockwise_circle": clockwise_circle,
    "ctr_clockwise_circle": ctr_clockwise_circle,
    "constant_speed": constant_speed,
    "weave": weave,
    "left_turn": left_turn,
    "left_u_turn": left_u_turn,
    "right_turn": right_turn,
    "right_u_turn": right_u_turn,
    "stationary" : stationary
}

motions = list(PIPELINE)

for motion in motions:
    test_data_raw_path = f"../drive_data/raw/test/{motion}"
    df = pd.read_csv(f"{test_data_raw_path}/{motion}.csv")

    test_data_clean_path = f"../drive_data/clean/test/{motion}/"
    print(f"Cleaning {motion}")
    PIPELINE[motion].clean(df, test_data_clean_path)

flag = "test"
#hard_accel_decel
print(f"Cleaning hard_accel_decel")
path = "../drive_data/raw/test/hard_accel_decel/hard_accel_decel.csv"
df = pd.read_csv(path)
hard_accel_decel.clean(df, flag)

#smooth_accel_decel
print(f"Cleaning smooth_accel_decel")
path = "../drive_data/raw/test/smooth_accel_decel/smooth_accel_decel.csv"
df = pd.read_csv(path)
smooth_accel_decel.clean(df, flag)