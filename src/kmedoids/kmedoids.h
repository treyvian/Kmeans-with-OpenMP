#ifndef KMEDOIDS_H

#define KMEDOIDS_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <omp.h>

/*
* Implementation of the k-medoids algorithms
*
* @pram points array of dimensions: n x dimensions, containing the data in input
* @param clusters array of dimension n which at the end of the algorithm will * * containt the number of the cluster associated to the point
* @param n number of points in input
* @param dimensions dimension of the point in input
* @param k number of cluster to divide the dataset into
*/
void k_medoids (double **points,
                int *clusters,
                const int n,
                const int dimensions,  
                const int k);

#endif