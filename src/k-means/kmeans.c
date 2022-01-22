#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

#include "kmeans.h"

void k_means (point *points, 
                const int n, 
                const int epochs, 
                const int k) {
    
    assert(points != NULL);

    assert(epochs > 0);

    point *centroids = (point *)calloc(k, sizeof(point));
    assert(centroids != NULL);

    int bool, random, iter, cluster_num;
    int points_dimensions = points->dimensions;

    srand(time(0));
    for (int i = 0; i < k; i++) {
        bool = 1;
        random = rand() % n;
        for (int j = 0; j < i; j++) {
            if (equals(&points[random], &centroids[j])) {
                bool = 0;
                i--;
                break;
            }    
        }
        if (bool) {
            copy_point(&centroids[i], &points[random]);
        }
    }
    
    int *nPoints = (int *)malloc(k * sizeof(int));
    assert(nPoints != NULL);

    double **sum = (double **)malloc(k * sizeof(double *));
    assert(sum != NULL);

    for (int i = 0; i < k; ++i) {
	    sum[i] = (double *)malloc(points_dimensions * sizeof(double));
        assert(sum[i] != NULL);
	}
        
    for (int t = 0; t<epochs; t++) {
        double distance;

        // Initialise with zeroes
        for (int j = 0; j < k; ++j) {
            nPoints[j] = 0;
            for (int i = 0; i< points_dimensions; i++) {
                sum[j][i] = 0.0;
            }    
        }

        // Updating the distance of each point with respect to the current centroids
        #pragma omp parallel for private(distance, cluster_num) schedule(static)
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < k; ++j) {
                distance = euclidian_distance(&centroids[j], &points[i]);

                if (distance < points[i].min_distance){
                    points[i].min_distance = distance;
                    points[i].cluster = j;
                    cluster_num = j; 
                }
            }

            // Iterate over points to append data to centroids
            #pragma omp critical 
            {
                nPoints[cluster_num]++;
            
                for (int j = 0; j < points_dimensions; ++j) {
                    sum[cluster_num][j] += points[i].x[j]; 
                }
            
                points[i].min_distance = __DBL_MAX__;
            }
        }
        
        bool = 1;

        // Compute new centroids
        double prov_sum;
        for (int i = 0; i < k; ++i) {
            int nP = nPoints[i];

            for (int j = 0; j < points_dimensions; j++) {
                prov_sum = sum[i][j]/nP;
                if (centroids[i].x[j] != prov_sum) {
                    bool = 0;
                    centroids[i].x[j] = prov_sum;
                }    
            }
        }

        if (bool) {
            // Freeing points, sum and centroids
            for (int i = 0; i < k; ++i) {
                free(sum[i]);
                delete_point(&centroids[i]);
            }
            free(sum);
            free(centroids); 
            free(nPoints); 
            
            return;
        }

        iter = t + 1;
    }
    
    // Freeing points, sum and centroids
    for (int i = 0; i < k; ++i) {
        free(sum[i]);
        delete_point(&centroids[i]);
    }
    free(sum);
    free(centroids); 
    free(nPoints);  
}