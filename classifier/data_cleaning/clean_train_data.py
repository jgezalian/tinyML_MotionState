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
    train_data_raw_path = f"../drive_data/raw/train/{motion}"
    df = pd.read_csv(f"{train_data_raw_path}/{motion}.csv")

    train_data_clean_path = f"../drive_data/clean/train/{motion}/"
    print(f"Cleaning {motion}")
    PIPELINE[motion].clean(df, train_data_clean_path)

flag = "train"
#hard_accel_decel
print(f"Cleaning hard_accel_decel")
path = "../drive_data/raw/train/hard_accel_decel/hard_accel_decel.csv"
df = pd.read_csv(path)
hard_accel_decel.clean(df, flag)

#smooth_accel_decel
print(f"Cleaning smooth_accel_decel")
path = "../drive_data/raw/train/smooth_accel_decel/smooth_accel_decel.csv"
df = pd.read_csv(path)
smooth_accel_decel.clean(df, flag)