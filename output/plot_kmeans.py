import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns


plt.figure()
df = pd.read_csv("output_mall_kmeans.csv",sep=',')
plt.scatter(df['X'],df['Y'],c=df['Cluster'],cmap='rainbow')

plt.show()
