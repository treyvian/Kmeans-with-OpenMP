from sklearn.decomposition import PCA
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv("housing.csv",sep=',')

pca = PCA().fit(data)

plt.figure()
plt.plot(np.cumsum(pca.explained_variance_ratio_), "-o")
plt.xlabel('Number of Components')
plt.ylabel('Variance (%)') #for each component
plt.show()