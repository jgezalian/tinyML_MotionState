import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier

data = pd.read_csv("./combined.csv")
sensor_cols = ["x_g", "y_g", "z_g", "x_dps", "y_dps", "z_dps"]
target_col = "orientation"

sensor_data, target = data[sensor_cols], data[target_col]
data_train, data_test, target_train, target_test = train_test_split(sensor_data, target, random_state=0)

tree = DecisionTreeClassifier(max_depth=1)
tree.fit(data_train, target_train)

test_state = pd.DataFrame({"x_g" : [1], "y_g" : [0], "z_g" : [0], "x_dps" : [0], "y_dps" : [0], "z_dps" : [0]})
print(tree.predict(test_state))