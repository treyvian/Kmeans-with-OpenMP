#include "kmeans.h"

int point_dimension;

double euclidian_dist (const double *p1, const double *p2) {

    double distance = 0;

    for (int i = 0; i < point_dimension; ++i) {
        distance += pow((p2[i] - p1[i]),2);
    }
    
    return sqrt(distance);
}


void k_means (double **points, 
                int *clusters,
                const int n, 
                const int dimensions,
                const int epochs, 
                const int k) {
    
    assert(points != NULL);
    assert(clusters != NULL);
    assert(epochs > 0);

    point_dimension = dimensions;

    double centroids[k][point_dimension];

    int n_points[k];

    double sum[k][dimensions];


    for (int i = 0; i < k; i++) {
        n_points[i] = 0;
        for (int j = 0; j < point_dimension; j++){
            centroids[i][j] = points[i][j];
            sum[i][j] = 0.0;
        }
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
                distance = euclidian_dist(centroids[j], points[i]);
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
                #pragma omp atomic
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
}
