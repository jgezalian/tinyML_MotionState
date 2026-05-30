import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

#hard_accel_decel
csv = "../drive_data/raw/hard_accel_decel/filtered_dps_z_hard_accel_decel.csv"
df = pd.read_csv(csv)

df["time_sec_segment"] = (df["time_sec"] - df.groupby("segment_id")["time_sec"].transform(lambda s : s.iloc[0])).round(3)
df.to_csv("../drive_data/clean/hard_accel_decel/hard_accel_decel.csv", index=False)



attributes = ["a_x", "a_y", "a_z", "dps_x", "dps_y", "dps_z"]

def plot_all_attributes(attributes):
    for attribute in attributes:
        fig = plt.figure(figsize=(20,10))
        for segment_id, segment in df.groupby("segment_id"):
            plt.plot(segment["time_sec_segment"], segment[attribute])
        plt.xlabel("time_sec")
        plt.ylabel(attribute)
        plt.title(attribute)
        fig.canvas.manager.set_window_title(attribute)
        plt.grid(True)
        plt.savefig(f"../drive_data/clean/hard_accel_decel/{attribute}.png")

plot_all_attributes(attributes)