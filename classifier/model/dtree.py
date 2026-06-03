import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import cross_val_predict
from sklearn.metrics import confusion_matrix
from sklearn.metrics import precision_score, recall_score
from sklearn import tree
from sklearn.tree import export_graphviz
from graphviz import Source

features_csv =  "../features/master_features.csv"
labels_csv = "../features/master_labels.csv"

X = pd.read_csv(features_csv)
y = pd.read_csv(labels_csv)

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.25, random_state=0)
clf = tree.DecisionTreeClassifier(max_depth=5, random_state=42, class_weight="balanced")
#clf.fit(X_train, y_train)

y_train_pred = cross_val_predict(clf, X_train, y_train, cv=3)
cm = confusion_matrix(y_train, y_train_pred)
print(cm)

print(precision_score(y_train, y_train_pred, average="weighted"))
print(recall_score(y_train, y_train_pred, average="weighted"))

# export_graphviz(
#     clf,
#     out_file="tree.dot",
#     feature_names=X.columns,
#     class_names = pd.unique(y["label"]),
#     rounded=True,
#     filled=True
# )

# Source.from_file("tree.dot")
# scores = cross_val_score(clf, X, y, cv=5)
# print(scores)