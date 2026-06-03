import pandas as pd

discrete = pd.read_csv("discrete.csv")
continuous = pd.read_csv("continuous.csv")
bumps = pd.read_csv("bump_test_features.csv")

# Remove exported index column
#discrete = discrete.drop(columns=["Unnamed: 0"], errors="ignore")
#continuous = continuous.drop(columns=["Unnamed: 0"], errors="ignore")
bumps = bumps.drop(columns=["Unnamed: 0"], errors="ignore")

# Optional but recommended for first model:
# remove incomplete continuous windows
#continuous = continuous[continuous["num_samples"] == 40].copy()

#master = pd.concat([discrete, continuous], ignore_index=True)
metadata_cols = ["source_dataset", "segment_id"]

X = bumps.drop(columns=metadata_cols)
X.to_csv("bumps_test_ready.csv", index=False)