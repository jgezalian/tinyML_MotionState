import pandas as pd

discrete = pd.read_csv("discrete.csv")
continuous = pd.read_csv("continuous.csv")

# Remove exported index column
discrete = discrete.drop(columns=["Unnamed: 0"], errors="ignore")
continuous = continuous.drop(columns=["Unnamed: 0"], errors="ignore")

# Optional but recommended for first model:
# remove incomplete continuous windows
continuous = continuous[continuous["num_samples"] == 40].copy()

master = pd.concat([discrete, continuous], ignore_index=True)

metadata_cols = ["source_dataset", "segment_id", "label"]

X = master.drop(columns=metadata_cols)
y = master["label"]

X.to_csv("master_features.csv", index=False)
y.to_csv("master_labels.csv", index=False)