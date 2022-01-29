#include "kmeans.h"

void k_means (double **points, 
                int *clusters,
                const int n, 
                const int dimensions,
                const int epochs, 
                const int k) {
    
    assert(points != NULL);
    assert(clusters != NULL);
    assert(epochs > 0);

    double **centroids = (double **)calloc(k, sizeof(double *));
    assert(centroids != NULL);

    int *n_points = (int *)calloc(k, sizeof(int));
    assert(n_points != NULL);

    double **sum = (double **)calloc(k, sizeof(double *));
    assert(sum != NULL);


    for (int i = 0; i < k; i++) {
        centroids[i] = (double *)calloc(dimensions, sizeof(double));
        assert(centroids[i]);
        
        centroids[i] = points[i];

        sum[i] = (double *)calloc(dimensions, sizeof(double));
        assert(sum[i] != NULL);        
    }

    
    double tstart, elapsed;
    int boolean = 1;
    double p_distance, distance;
    int cluster_num;

    #pragma omp parallel
    for (int t = 0; boolean && t < epochs; t++) {

        // Updating the distance of each point with respect to the current centroids
        #pragma omp for private(p_distance, distance, cluster_num) firstprivate(n, k, centroids, dimensions, points)
        for (int i = 0; i < n; ++i) {
            p_distance = __DBL_MAX__;

            
            for (int j = 0; j < k; ++j) {
                distance = euclidian_distance(centroids[j], points[i], dimensions);
                if (distance < p_distance){
                    p_distance = distance;
                    cluster_num = j; 
                }
            }

            clusters[i] = cluster_num;
            
            #pragma omp atomic
            n_points[cluster_num]++;
            
            // Iterate over points to append data to centroids
            for (int j = 0; j < dimensions; ++j) {
                sum[cluster_num][j] += points[i][j];
            }
            
        }
                
        boolean = 1;
    
        // Compute new centroids
        #pragma omp for firstprivate(k, dimensions)
        for (int i = 0; i < k; ++i) {
            int n_p = n_points[i];
            n_points[i] = 0;

            for (int j = 0; j < dimensions; j++) {
                double prov_sum = sum[i][j]/n_p;
                sum[i][j] = 0.0;
                if (centroids[i][j] != prov_sum) { 
                    boolean = 1;
                    centroids[i][j] = prov_sum;
                }    
            }
        }
    }

    
    //Freeing points, sum and centroids
    for (int i = 0; i < k; ++i) {
        free(sum[i]);
    }
    free(n_points);
    free(sum);
    free(centroids);
}
