import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from scipy.signal import find_peaks


def plot_all_attributes(df, path):
        fig = plt.figure(figsize=(20, 5))
        ax = plt.gca()
        x = df.iloc[0:1000]["timestamp"]/1000
        y = df.iloc[0:1000]["a_x"]
        ax.plot(x, y)
        ax.set_xticks(np.arange(x.min(), x.max() + 1, 5))
        ax.tick_params(axis="x", labelsize=8, labelrotation=45)
        plt.setp(ax.get_xticklabels(), ha="right")
        plt.xlabel("time_sec")
        plt.ylabel("a_x")
        plt.title("a_x")
        fig.canvas.manager.set_window_title("a_x")
        plt.grid(True)
        plt.savefig(path)
        plt.close()

df = pd.read_csv("../../drive_data/raw/train/hard_accel_decel/hard_accel_decel.csv")
plot_all_attributes(df, "../../drive_data/raw/train/hard_accel_decel/a_x.png")
