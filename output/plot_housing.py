import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("output_house_kmeans.csv",sep=',')

plt.figure()
plt.title("kmeans output")
plt.scatter(df['Longitude'], df['Latitude'], c=df['Cluster'],cmap='rainbow')

df = pd.read_csv("output_housing_kmedoids.csv",sep=',')

plt.figure()
plt.title("kmedoids output")
plt.scatter(df['Longitude'], df['Latitude'], c=df['Cluster'],cmap='rainbow')
plt.show()

