#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "kmeans.h"


void kMeansClustering (point *points, 
                        const int n, 
                        const int epochs, 
                        const int k) {

    if (points == NULL){
        printf("Missing input data");
    }
    
    point *centroids = (point *)malloc(k * sizeof(point));

    srand(time(0));
    for (int i = 0; i < k; ++i) {
        centroids[i] = points[rand() % n];
    }
    
    int *nPoints = (int *)malloc(k * sizeof(int));
    double **sum = (double **)malloc(k * sizeof(double *));

    for (int i = 0; i < k; ++i) {
	    sum[i] = (double *)malloc(points->dimensions * sizeof(double));
	}
    
    int iter;

    for (int t = 0; t<epochs; t++) {

        double dist;
        int cluster_id;

        for (int i = 0; i < k; ++i) {
            cluster_id = i;

            for (int j = 0; j < n; j++) {
                dist = distance(centroids[i], points[j]);
        
                if (dist < points[j].minDist) {
                    points[j].minDist = dist;
                    points[j].cluster = cluster_id;
                }
            }
        }
    
        // Initialise with zeroes
        for (int j = 0; j < k; ++j) {
            nPoints[j] = 0;
            for (int i = 0; i< points->dimensions; i++) {
                sum[j][i] = 0.0;
            }    
        }
        
        // Iterate over points to append data to centroids
        for (int i=0; i<n; i++) {
            int cluster_id = points[i].cluster;
            nPoints[cluster_id] += 1;
            
            for (int j = 0; j < points->dimensions; ++j) {
                sum[cluster_id][j] += points[i].x[j]; 
            }

            points[i].minDist = __DBL_MAX__;
        }
        /*
            Boolean variable used to stop the loop in case there 
            is no improvement in the new centroids
        */
        int bool = 1;

        // Compute new centroids
        for (int i=0; i<k; ++i) {
            int nP = nPoints[i];

            for (int j = 0; j<centroids->dimensions; j++) {
                if (centroids[i].x[j] != sum[i][j]/nP) {
                    bool = 0;
                    centroids[i].x[j] = sum[i][j]/nP;
                }    
            }
        }

        if (bool) {
            printf("Number of iterations: %d\n", iter);
            return;
        }

        iter = t;
    }
    printf("%d\n", iter);
}
 