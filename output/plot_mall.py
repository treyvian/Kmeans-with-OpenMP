import matplotlib.pyplot as plt
import pandas as pd

plt.figure()
plt.title("kmeans output")
df = pd.read_csv("output/output_mall_kmeans.csv",sep=',')
plt.scatter(df['X'],df['Y'],c=df['Cluster'],cmap='rainbow')

plt.figure()
plt.title("kmedoids output")
df = pd.read_csv("output/output_mall_kmedoids.csv",sep=',')
plt.scatter(df['X'],df['Y'],c=df['Cluster'],cmap='rainbow')

plt.show()
