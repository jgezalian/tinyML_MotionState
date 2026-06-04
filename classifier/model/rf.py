import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import cross_val_predict
from sklearn.metrics import confusion_matrix
from sklearn.metrics import precision_score, recall_score
from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import ExtraTreesClassifier

train_features_csv = "../features/train/master_train_features.csv"
test_features_csv = "../features/test/master_test_features.csv"

train = pd.read_csv(train_features_csv)
test = pd.read_csv(test_features_csv)
test_classes = test["label"]
test_features = test.drop(columns="label")

X = train.drop(columns=["label"])
y = pd.read_csv(train_features_csv)["label"]

X_train, X_test, y_train, y_test = train_test_split(
    X,
    np.ravel(y),
    test_size=0.25,
    random_state=0,
    stratify=np.ravel(y)
)

rnd_clf = RandomForestClassifier(
    n_estimators=500, max_depth=5, n_jobs=-1, random_state=42, #class_weight="balanced"
)

rnd_clf.fit(X_train, (y_train))

predicted_classes = rnd_clf.predict(test_features)

results = pd.DataFrame({
    "actual": test_classes,
    "predicted": predicted_classes,
})
results.to_csv("predicted_classes.csv", index=False)



