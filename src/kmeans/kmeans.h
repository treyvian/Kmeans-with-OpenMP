#ifndef KMEANS_H

#define KMEANS_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <omp.h>

#include "../point/point.h"

void k_means (point *, 
                const int, 
                const int, 
                const int);

#endif