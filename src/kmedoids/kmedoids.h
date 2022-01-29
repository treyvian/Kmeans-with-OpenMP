#ifndef KMEDOIDS_H

#define KMEDOIDS_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

#include "../silhouette-score/silhouette.h"

void k_medoids (double **points,
                int *clusters,
                const int n,
                const int dimensions,  
                const int);

#endif