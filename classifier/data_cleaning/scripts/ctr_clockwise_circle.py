import pandas as pd
import matplotlib.pyplot as plt
import numpy as np



def plot_all_attributes(attributes, df, train_data_clean_path):
    for attribute in attributes:
        fig = plt.figure()
        fig.canvas.manager.set_window_title(attribute)
        plt.plot(df["time_sec"], df[attribute])
        plt.xlabel("time_sec")
        plt.ylabel(attribute)
        plt.title(attribute)
        plt.savefig(f"{train_data_clean_path}/{attribute}.png")
        plt.close()


def clean(df, train_data_clean_path):

    time_col = "timestamp"
    df["time_sec"] = (df[time_col] - df[time_col].iloc[0]) / 1000
    df.to_csv(f"{train_data_clean_path}/ctr_clockwise_circle.csv")

    attributes = ["a_x", "a_y", "a_z", "dps_x", "dps_y", "dps_z"]

    plot_all_attributes(attributes, df, train_data_clean_path)
