import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
import pandas as pd
import seaborn as sns

df = pd.read_csv("output.csv",sep=',')

fig = plt.figure(figsize = (12,8))
ax = plt.axes(projection='3d')


sctt = ax.scatter3D(df['Longitude'], df['Latitude'], df['MedInc'], c=df['Cluster'],cmap='rainbow')
plt.show()


