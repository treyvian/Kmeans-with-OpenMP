#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "kmeans.h"


void kMeansClustering (point *points, 
                        const int n, 
                        const int epochs, 
                        const int k) {
    
    if (points == NULL){
        perror("Missing input data");
        exit(-1);
    }

    if (epochs <= 0) {
        perror("Number of epochs not correct");
        exit(-1);
    }

    for (int i = 0; i < n; i++) {
        reset_point(&points[i]);
    }
    
    point *centroids = (point *)malloc(k * sizeof(point));

    srand(time(0));
    for (int i = 0; i < k; ++i) {
        copy_point(&points[rand() % n], &centroids[i]);
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

        // Updating the distance of each point with respect to the current centroids
    
        for (int i = 0; i < k; ++i) {
            cluster_id = i;

            for (int j = 0; j < n; j++) {
                dist = distance(&centroids[i], &points[j]);
        
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
        int cluster_num;
        for (int i=0; i<n; i++) {
            cluster_num = points[i].cluster;
            ++nPoints[cluster_num];
            
            for (int j = 0; j < points->dimensions; ++j) {
                sum[cluster_num][j] += points[i].x[j]; 
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
            // Freeing points, sum and centroids
            free(nPoints);

            for (int i = 0; i < k; ++i) {
                free(sum[i]);
            }
            free(sum);

            for (int i = 0; i < k; ++i) {
                delete_x(&centroids[i]);
            }
            free(centroids);

            printf("Total n° of iterations with %d clusters: %d\n", k, iter);
            return;
        }

        iter = t + 1;
    }

    printf("Total n° of iterations with %d clusters: %d\n", k, iter);
    // Freeing points, sum and centroids
    free(nPoints);

    for (int i = 0; i < k; ++i) {
        free(sum[i]);
    }
    free(sum);

    for (int i = 0; i < k; ++i) {
        delete_x(&centroids[i]);
    }
    free(centroids);
}


double silhouette_score (point *data, int n) {
    double Cohesion;
    int n_coh;
    double Separation[n];
    int n_sep;
    double silhouette_score = 0;
    int cluster_number;

    for (int i = 0; i < n; ++i){
        Separation[i] = 0.0; 
    }

    for (int i=0; i < n; ++i) {
        Cohesion = 0;
        n_coh = 0;
        n_sep = 0;
        cluster_number = data[i].cluster;
        
        for (int j = 0; j < n; ++j) {
            if (&data[i] == &data[j]) {            
                continue;
            } else if (cluster_number == data[j].cluster) {
                Cohesion += distance(&data[i], &data[j]);
                ++n_coh;
            } else {
                Separation[i] = distance(&data[i], &data[j]);
                ++n_sep;
            }
        }

        double sep = __DBL_MAX__;
        for (int j = 0; j < n; ++j) {
            if (Separation[j] != 0 && sep > Separation[j]){
                sep = Separation[j];
            }
        }

        double mean_coh = Cohesion / (n_coh - 1);
        double mean_sep = sep / n_sep;

        if (mean_sep > mean_coh) {
            silhouette_score += (mean_sep - mean_coh) / mean_sep;
        } else silhouette_score += (mean_sep - mean_coh) / mean_coh;
    }
    return silhouette_score / n;
}
