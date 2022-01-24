#ifndef SILHOUETTE_H

#define SILHOUETTE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

#include "../point/point.h"

double silhouette_score (point *, int, int);

#endif