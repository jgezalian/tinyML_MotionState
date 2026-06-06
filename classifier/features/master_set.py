import pandas as pd

discrete_test = pd.read_csv("./test/discrete_features.csv")
discrete_train = pd.read_csv("./train/discrete_features.csv")
continuous_test = pd.read_csv("./test/continuous_features.csv")
continuous_train = pd.read_csv("./train/continuous_features.csv")

# Remove exported index column
discrete_test = discrete_test.drop(columns=["Unnamed: 0"], errors="ignore")
discrete_train = discrete_train.drop(columns=["Unnamed: 0"], errors="ignore")
continuous_test = continuous_test.drop(columns=["Unnamed: 0"], errors="ignore")
continuous_train = continuous_train.drop(columns=["Unnamed: 0"], errors="ignore")


# Optional but recommended for first model:
# remove incomplete continuous windows
# continuous = continuous[continuous["num_samples"] == 40].copy()

master_test = pd.concat([discrete_test, continuous_test], ignore_index=True)
master_train = pd.concat([discrete_train, continuous_train], ignore_index=True)

X_train = master_train
X_test = master_test
X_train.to_csv("./train/master_train_features.csv", index=False)
X_test.to_csv("./test/master_test_features.csv", index=False)
