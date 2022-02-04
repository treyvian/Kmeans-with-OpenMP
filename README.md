# Kmeans-with-OpenMP

Project for Architectures for Artificial Intelligence module 2.

[ProjectExam.pdf](ProjectExam.pdf) for more information about the request.

- [Abstract](#abstract)
- [Technologies](#tech)
- [Requisites](#requisites)
- [Usage](#usage)
- [Clustering Algorithms](#clustering-algorithms)
  - [k-means](#k-means)
  - [k-means](#k-medoids)
- [Datasets](#datasets)
  - [Mall Customer](#mall-customer)
  - [California Housing Prices](#california-housing-prices)

## Abstract


## Technologies

This project is built using:

- [OpenMp](https://www.openmp.org/) - an API that supports multi-platform shared-memory multiprocessing programming in C, C++, and Fortran.

## Requisites

- `gcc (GCC)` v11.1
- `OpenMp` v13

In case you want to run the two python files in for visualizing the output you will need `matplotlib` and `pandas` installed. The path are inside the python files are relative so launch the file inside the output folder.

## Usage

In order to run the project you can use the two bash files in the root directory.

To execute the one relative to the mall customer dataset run 

```
./launch_mall
```

The execute the one relative to the California housing dataset run 

```
./launch_house
```

Both will run the Makefiles and generate the output inside the [output](output/) directory in the root folder.

## Clustering Algorithms

### K-means

The [k-means](https://en.wikipedia.org/wiki/K-means_clustering)  is a method of vector quantization, that aims to partition n observations into k clusters in which each observation belongs to the cluster with the nearest mean (cluster centers or cluster centroid), serving as a prototype of the cluster.

In my implementation there is an external for loop with dependencies, so not parallelizable, and two inner loop which are parallelized separately, the first one is used to calculate the distance (using the Euclidian metric) of each point from the current centroids and the other one updates the centroids value. The outer loop stops when the max number of iterations is reached. 

### k-medoids

The k-medoids problem is a clustering problem similar to k-means. In contrast to the k-means algorithm, k-medoids chooses actual data points as centers (medoids). k-medoids can be used with arbitrary dissimilarity measures, whereas k-means generally requires Euclidean distance for efficient solutions. Because k-medoids minimizes a sum of pairwise dissimilarities instead of a sum of squared Euclidean distances, it is more robust to noise and outliers than k-means. 

My implementation does not use any strategies for the optimization of the search, it's a naive implementation recomputing the entire cost function every time and having a runtime complexity of O(n<sup>2</sup> k<sup>2</sup>). This high runtime complexity can be observed in the California dataset with the high number of entries. In this case I used the Manhattan distance to calculate the total cost.

## Datasets

I've used two dataset for this project, both taken from [keggle](https://www.kaggle.com/)

### Mall Customer

The [Mall Customer](https://www.kaggle.com/vjchoudhary7/customer-segmentation-tutorial-in-python) is created only for the learning purpose of the customer segmentation concepts , also known as market basket analysis. 

This is a quiet small dataset containing 200 rows and 5 columns. I only use the last two dimensions/columns of the dataset("Annual income","spending score") since they are well divided and easy to visualize. This allowed me to check the correctness of my implementations thanks to the immediate feedback from the plot.

The number of clusters which gives the highest silhouette score is 5 which can be easily confirmed by looking at the data distribution.

### California Housing Prices

The [California Housing Prices](https://www.kaggle.com/camnugent/california-housing-prices) dataset contains information from the 1990 California census. 

This dataset is bigger with respect to the other one, it contains more than 20k rows and 9 columns. In this case I took 3 columns("Longitude","Latitude","Median Income") to further test my implementations. While the k-means implementation is not bothered by the increased size of the dataset while the k-medoids has an hard time with its O(n<sup>2</sup> k<sup>2</sup>) runtime complexity. This will make more room for improvement when parallelization is introduced.

Based on the silhouette score the best number of clusters is 2, but all the notebooks that I found on Keggle use the 6 as number of clusters. In this case the fixed number of cluster is preferable also to avoid waiting around for the k-medoids algorithm to finish all the possible combinations.

The csv file in the data folder of the project corresponding to this dataset, has been preprocessed by removing all the columns that were not necessary leaving only the three that we interested in.