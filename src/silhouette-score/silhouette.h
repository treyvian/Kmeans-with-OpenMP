#ifndef SILHOUETTE_H

#define SILHOUETTE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <omp.h>

double silhouette_score (double **points, int *clusters, const int n, const int dimensions, const int k);

#endif