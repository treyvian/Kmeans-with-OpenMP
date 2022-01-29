#ifndef KMEANS_H

#define KMEANS_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <omp.h>

#include "../silhouette-score/silhouette.h"

void k_means (double **points,
                int *clusters,
                const int n,
                const int dimensions, 
                const int epochs,
                const int k);

#endif