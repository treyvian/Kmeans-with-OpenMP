#ifndef SILHOUETTE_H

#define SILHOUETTE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <omp.h>

/*
* Calculate the euclidian distance between 2 points
*
* @param p1 first point to take in consideration
* @param p2 second point to take in consideration
* @return the distance between p1 and p2
*/
double euclidian_distance (const double *p1, const double *p2, int dimensions);

/*
* Calculate the manhattan distance between 2 points
*
* @param p1 first point to take in consideration
* @param p2 second point to take in consideration
* @return the distance between p1 and p2
*/
double manhattan_distance (const double *p1, const double *p2, int dimensions);

double silhouette_score (double **points, int *clusters, int n, int dimensions, int k);

#endif