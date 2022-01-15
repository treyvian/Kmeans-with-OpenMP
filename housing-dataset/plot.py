import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
import pandas as pd
import seaborn as sns

df = pd.read_csv("output.csv",sep=',')

plt.figure()
plt.scatter(df['Longitude'], df['Latitude'], c=df['Cluster'],cmap='rainbow')
plt.show()


