import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

#ctr_clock_loop
csv = "../drive_data/raw/ctr_clockwise_circle/ctr_clockwise_circle.csv"
df = pd.read_csv(csv)
time_col = "timestamp"
df["time_sec"] = (df[time_col] - df[time_col].iloc[0]) / 1000
df.to_csv("../drive_data/clean/ctr_clockwise_circle/ctr_clockwise_circle.csv")

attributes = ["a_x", "a_y", "a_z", "dps_x", "dps_y", "dps_z"]

def plot_all_attributes(attributes):
    for attribute in attributes:
        fig = plt.figure()
        fig.canvas.manager.set_window_title(attribute)
        plt.plot(df["time_sec"], df[attribute])
        plt.xlabel("time_sec")
        plt.ylabel(attribute)
        plt.title(attribute)
        plt.savefig(f"../drive_data/clean/ctr_clockwise_circle/{attribute}.png")

plot_all_attributes(attributes)