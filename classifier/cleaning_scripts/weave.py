import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

#smooth_constant_speed
csv = "../drive_data/raw/weave/weave.csv"
df = pd.read_csv(csv)

df["time_sec"] = (df["timestamp"] - df.iloc[0]["timestamp"]) / 1000

df.to_csv("../drive_data/clean/weave/weave.csv")