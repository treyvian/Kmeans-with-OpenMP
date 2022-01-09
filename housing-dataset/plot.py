import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns

X = pd.read_csv("output.csv",sep=',')

sns.relplot(x=X["Longitude"], y=X["Latitude"], hue=X["Cluster"], data=X, height=6,)

X["MedHouseVal"] = df["MedHouseVal"]
sns.catplot(x=X["MedInc"], y=X["Cluster"], data=X, kind="boxen", height=6)

