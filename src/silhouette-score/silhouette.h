#ifndef SILHOUETTE_H

#define SILHOUETTE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <omp.h>

/*
* Implementation of the silhouette algorithms
*
* @pram points array of dimensions: n x dimensions, contains the data in input
* @param clusters n-dimensional array containts the number of the cluster 
* associated to the point
* @param n number of points in input
* @param dimensions dimension of the point in input
* @param k number of cluster to divide the dataset into
*/
double silhouette_score (double **points, int *clusters, const int n, const int dimensions, const int k);

#endif