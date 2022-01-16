# Kmeans-with-OpenMP
Project for Architectures for Artificial Intelligence module 2.

[ProjectExam.pdf](ProjectExam.pdf) for more information about the request.

- [Abstract](##Abstract)
- [Technologies](##Tech)
- [Requisites](##Requisites)
- [Clustering Algorithms](##Clustering-Algorithms)
- [Datasets](##Datasets)

## Abstract


## Technologies
This project is built using:

- [Openmp](https://www.openmp.org/) - an API that supports multi-platform shared-memory multiprocessing programming in C, C++, and Fortran.

## Clustering Algorithms
### K-means
The [k-means](https://en.wikipedia.org/wiki/K-means_clustering)  is a method of vector quantization, that aims to partition n observations into k clusters in which each observation belongs to the cluster with the nearest mean (cluster centers or cluster centroid), serving as a prototype of the cluster.

In my implementation I use as metric the euclidian distance.

### k-medoids
The k-medoids problem is a clustering problem similar to k-means. In contrast to the k-means algorithm, k-medoids chooses actual data points as centers (medoids). k-medoids can be used with arbitrary dissimilarity measures, whereas k-means generally requires Euclidean distance for efficient solutions. Because k-medoids minimizes a sum of pairwise dissimilarities instead of a sum of squared Euclidean distances, it is more robust to noise and outliers than k-means. 

My implementation does not use any strategies for optimazing the search, it's a naive implementation recomputing the entire cost function every time and having a runtime complexity of O(n<sup>2</sup> k<sup>2</sup>). 

[TODO]: #

## Datasets
### Mall Customer
The [Mall Customer](https://www.kaggle.com/vjchoudhary7/customer-segmentation-tutorial-in-python) is created only for the learning purpose of the customer segmentation concepts , also known as market basket analysis. 

### California Housing Prices
The [California Housing Prices](https://www.kaggle.com/camnugent/california-housing-prices) dataset contains information from the 1990 California census. 

All the code is checked with [Valgrind](https://valgrind.org/) to prevent memory leaks.