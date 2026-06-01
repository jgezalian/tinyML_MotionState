import pandas as pd
import numpy as np
from scipy.signal import find_peaks
import matplotlib.pyplot as plt

def zero_crossings(segment, attribute, window=5):
    print(segment.iloc[0]["segment_id"])
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

    #print(signs)
    signs = signs[signs != 0]

    if len(signs) < 2:
        return 0
    
    #print((signs[1:] != signs[:-1]))
    #print(np.nonzero((signs[1:] != signs[:-1])))
    return {f"{attribute}_zero_crossings" : int(np.sum(signs[1:] != signs[:-1]))}
    


df = pd.read_csv(f"../drive_data/clean/hard_accel_decel/hard_accel_decel.csv")
for segment_id, segment in df.groupby("segment_id"):
    print(zero_crossings(segment, "a_x"))
    fig = plt.figure()
    fig.canvas.manager.set_window_title("a_x")
    plt.plot(segment["segment_time_sec"], segment["a_x"])
    plt.xlabel("segment_time_sec")
    plt.ylabel("a_x")
    plt.title("a_x")
    plt.grid(True)
    plt.yticks(np.arange(-1, 1, 0.1))
    plt.show()
    




