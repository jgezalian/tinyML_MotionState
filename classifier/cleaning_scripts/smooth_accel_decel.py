import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

#smooth_accel_decel
csv = "../drive_data/raw/smooth_accel_decel/filtered_dps_z_smooth_accel_decel.csv"
df = pd.read_csv(csv)

df["time_sec"] = (df["timestamp"] - df.iloc[0]["timestamp"]) / 1000

df["segment_time_sec"] = (df["time_sec"] - df.groupby("segment_id")["time_sec"].transform(lambda s : s.iloc[0])).round(3)
df.to_csv("../drive_data/clean/smooth_accel_decel/smooth_accel_decel.csv", index=False)

attributes = ["a_x", "a_y", "a_z", "dps_x", "dps_y", "dps_z"]

def plot_all_attributes(attributes):
    for attribute in attributes:
        fig = plt.figure(figsize=(20,10))
        for segment_id, segment in df.groupby("segment_id"):
            plt.plot(segment["segment_time_sec"], segment[attribute])
        plt.xlabel("time_sec")
        plt.ylabel(attribute)
        plt.title(attribute)
        fig.canvas.manager.set_window_title(attribute)
        plt.grid(True)
        plt.savefig(f"../drive_data/clean/smooth_accel_decel/{attribute}.png")

plot_all_attributes(attributes)