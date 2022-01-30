import pandas as pd 

df = pd.read_csv("housing.csv")

df = df[['MedInc', 'Latitude', 'Longitude']]

df.to_csv('house_pre.csv', index=False)