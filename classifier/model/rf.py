import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import cross_val_predict
from sklearn.metrics import confusion_matrix
from sklearn.metrics import precision_score, recall_score
from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import ExtraTreesClassifier

features_csv = "../features/master_features_labeled.csv"
bumps_test_csv = "../features/bumps_test_ready.csv"
X_bumps_test = pd.read_csv(bumps_test_csv)
X_bumps_test = X_bumps_test.drop(columns=["label"])

X = pd.read_csv(features_csv)
X = X.drop(columns=["label"])
y = pd.read_csv(features_csv)["label"]

X_train, X_test, y_train, y_test = train_test_split(
    X,
    np.ravel(y),
    test_size=0.25,
    random_state=0,
    stratify=np.ravel(y)
)

rnd_clf = RandomForestClassifier(
    n_estimators=500, max_depth=5, n_jobs=-1, random_state=42, class_weight="balanced"
)

rnd_clf.fit(X_train, (y_train))
# y_pred_rf = rnd_clf.predict(X_test)

# rf_score = rnd_clf.score(X_test, y_test)
# print(rf_score)

predicted_classes = rnd_clf.predict(X_bumps_test)
print(predicted_classes)

# df = pd.DataFrame(
#     {
#         "y_test": y_test["label"].reset_index(drop=True),
#         "y_pred": pd.Series(y_pred_rf),
#     }
# )

# df.to_csv("compare.csv")
