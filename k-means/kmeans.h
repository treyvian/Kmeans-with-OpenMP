#ifndef KMEANS_H

#define KMEANS_H

#include "../point/point.h"

void k_means (point *, 
                        const int, 
                        const int, 
                        const int);

double silhouette_score (point *, int, int);

#endif