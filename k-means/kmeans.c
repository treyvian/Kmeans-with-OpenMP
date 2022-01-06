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
        exit(1);
    }

    if (epochs <= 0) {
        perror("Number of epochs not correct");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        reset_point(&points[i]);
    }

    point *centroids = (point *)malloc(k * sizeof(point));

    srand(time(0));
    int bool = 1;
    int random;
    for (int i = 0; i < k; i++) {
        random = rand() % n;
        for (int j = 0; j < i; j++) {
            if (equals(&points[random], &centroids[j])) {
                bool = 0;
                i--;
                break;
            }    
        }
        if (bool) {
            copy_point(&points[random], &centroids[i]);
        }
    }
    
    int *nPoints = (int *)malloc(k * sizeof(int));
    double **sum = (double **)malloc(k * sizeof(double *));

    for (int i = 0; i < k; ++i) {
	    sum[i] = (double *)malloc(points->dimensions * sizeof(double));
	}
    
    int iter;
    
    for (int t = 0; t<epochs; t++) {
        double dist;

                // Initialise with zeroes
        for (int j = 0; j < k; ++j) {
            nPoints[j] = 0;
            for (int i = 0; i< points->dimensions; i++) {
                sum[j][i] = 0.0;
            }    
        }

        // Updating the distance of each point with respect to the current centroids
        int cluster_num;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < k; ++j) {
                dist = distance(&centroids[j], &points[i]);

                if (dist < points[i].minDist){
                    points[i].minDist = dist;
                    points[i].cluster = j;
                    cluster_num = j; 
                }
            }

            // Iterate over points to append data to centroids
            nPoints[cluster_num]++;
            
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
        double prov_sum;
        for (int i = 0; i < k; ++i) {
            int nP = nPoints[i];

            for (int j = 0; j < centroids->dimensions; j++) {
                prov_sum = sum[i][j]/nP;
                if (centroids[i].x[j] != prov_sum) {
                    bool = 0;
                    centroids[i].x[j] = prov_sum;
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

            printf("With %d clusters, it ran for %d number of iterations\n", k, iter);
            return;
        }

        iter = t + 1;
    }

    printf("With %d clusters, it ran for %d number of iterations\n", k, iter);
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


double silhouette_score (point *data, int n, int k) {
    double Cohesion;
    double mean_coh;
    
    double Separation[k];
    double mean_sep;

    double silhouette_score = 0;
    int cluster_number;

    int n_clust[k];

    for (int i=0; i < n; ++i) {
        Cohesion = 0;
        cluster_number = data[i].cluster;
        
        for (int t = 0; t < k; ++t) {
            Separation[t] = 0.0;
            n_clust[t] = 0;
        }
        
        for (int j = 0; j < n; ++j) {
            if (i != j) {            
                if (cluster_number == data[j].cluster) {
                    Cohesion += distance(&data[i], &data[j]);
                    n_clust[data[j].cluster]++;
                
                } else {
                    Separation[data[j].cluster] += distance(&data[i], &data[j]);
                    n_clust[data[j].cluster]++;
                }
            }    
        }

        for (int y = 0; y < k; ++y) {
            if (n_clust[y] == 0) 
                printf("(%d, %f)", y, Separation[y]);
        }

        mean_coh = Cohesion / n_clust[cluster_number];
        
        mean_sep = __DBL_MAX__;
        double sep;
        for (int j = 0; j < k; ++j) {
            if (j != cluster_number) {
                sep = Separation[j] / n_clust[j];
                if (mean_sep > sep){
                    mean_sep = sep;
                }
            }    
        }

        if (sep > mean_coh) {
            silhouette_score += (mean_sep - mean_coh) / mean_sep;
        } else silhouette_score += (mean_sep - mean_coh) / mean_coh;
    }
    return silhouette_score / n;
}
