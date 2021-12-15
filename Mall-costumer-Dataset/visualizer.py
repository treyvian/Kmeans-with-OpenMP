import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns


plt.figure()
df = pd.read_csv("output.csv",sep=',')
sns.scatterplot(x=df.X, y=df.Y, 
                hue=df.Cluster, 
                palette=sns.color_palette("hls", n_colors=5))
plt.xlabel("Annual income (k$)")
plt.ylabel("Spending Score (1-100)")
plt.title("Clustered: spending (y) vs income (x)")

plt.show()
