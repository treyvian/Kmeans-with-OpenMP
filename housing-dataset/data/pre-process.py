import numpy as np
import pandas as pd
from sklearn.preprocessing import StandardScaler

data = pd.read_csv("housing.csv",sep=',')

features = ['Longitude', 'Latitude', 'MedInc']
df = data[features]

df.to_csv('data.csv')