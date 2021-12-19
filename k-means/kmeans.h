#ifndef KMEANS_H

#define KMEANS_H

#include "../point/point.h"

void kMeansClustering (point *, 
                        const int, 
                        const int, 
                        const int);

int silhouette_score (point *, int);

#endif