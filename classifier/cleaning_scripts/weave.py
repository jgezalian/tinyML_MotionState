import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

#smooth_constant_speed
csv = "../drive_data/raw/weave/weave.csv"
df = pd.read_csv(csv)

df["time_sec"] = (df["timestamp"] - df.iloc[0]["timestamp"]) / 1000

df.to_csv("../drive_data/clean/weave/weave.csv")

attributes = ["a_x", "a_y", "a_z", "dps_x", "dps_y", "dps_z"]

def plot_all_attributes(attributes):
    for attribute in attributes:
        fig = plt.figure(figsize=(20,10))
        fig.canvas.manager.set_window_title(attribute)
        plt.plot(df["time_sec"], df[attribute])
        plt.xlabel("time_sec")
        plt.ylabel(attribute)
        plt.title(attribute)
        plt.savefig(f"../drive_data/clean/weave/{attribute}.png")

plot_all_attributes(attributes)