import numpy as np
import pandas as pd
import emlearn
import json
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
    X, np.ravel(y), test_size=0.10, random_state=0, stratify=np.ravel(y)
)

rnd_clf = RandomForestClassifier(
    n_estimators=100, max_depth=7, n_jobs=-1, random_state=0, class_weight="balanced"
)

rnd_clf.fit(X_train, (y_train))

score = rnd_clf.score(test_features, test_classes)
print(score)

path = "rnd_clf.h"
cmodel = emlearn.convert(rnd_clf, method='inline', dtype="double")
cmodel.save(file=path, name='rnd_f')

meta_json = {}
meta_json["rf"] = {
    "n_estimators": rnd_clf.n_estimators,
    "max_depth": rnd_clf.max_depth,
    "n_jobs": rnd_clf.n_jobs,
    "random_state": rnd_clf.random_state,
}
meta_json["n_features"] = len(rnd_clf.feature_names_in_.tolist())
meta_json["features"] = rnd_clf.feature_names_in_.tolist()

meta_json["n_classes"] = len((rnd_clf.classes_).tolist())
meta_json["classes"] = (rnd_clf.classes_).tolist()

with open("meta.json", "w", encoding="utf-8") as f:
    json.dump(meta_json, f, ensure_ascii=False, indent=4)

predicted_classes = rnd_clf.predict(test_features)

results = pd.DataFrame({
    "actual": test_classes,
    "predicted": predicted_classes,
})
results.to_csv("predicted_classes.csv", index=False)
