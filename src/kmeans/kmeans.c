#include "kmeans.h"


void k_means (point *points, 
                const int n, 
                const int epochs, 
                const int k) {
    
    assert(points != NULL);

    assert(epochs > 0);

    point *centroids = (point *)calloc(k, sizeof(point));
    assert(centroids != NULL);

    int boolean, random;
    int points_dimensions = points->dimensions;

    srand(time(0));
    for (int i = 0; i < k; i++) {
        boolean = 1;
        random = rand() % n;
        
        for (int j = 0; j < i; j++) {
            if (equals(&points[random], &centroids[j])) {
                boolean = 0;
                i--;
                break;
            }    
        }
        if (boolean) {
            copy_point(&centroids[i], &points[random]);
        }
    }
    
    int *n_points = (int *)memalign(sizeof(int), k * sizeof(int));
    assert(n_points != NULL);

    double **sum = (double **)memalign(sizeof(double *), k * sizeof(double *));
    assert(sum != NULL);

    for (int i = 0; i < k; ++i) {
        sum[i] = (double *)memalign(sizeof(double), points_dimensions * sizeof(double));
        assert(sum[i] != NULL);
        
        n_points[i] = 0;
        for (int j = 0; j < points_dimensions; j++) {
            sum[i][j] = 0.0;
        }
	}
    
    boolean = 1;
    double p_distance, distance;
    int cluster_num;
    for (int t = 0; boolean && t < epochs; t++) {

        // Updating the distance of each point with respect to the current centroids
        #pragma omp parallel for private(p_distance, distance, cluster_num) firstprivate(n, k, centroids, points_dimensions)
        for (int i = 0; i < n; ++i) {
            p_distance = __DBL_MAX__;
            cluster_num;

            for (int j = 0; j < k; ++j) {
                distance = euclidian_distance(&centroids[j], &points[i]);
                if (distance < p_distance){
                    p_distance = distance;
                    cluster_num = j; 
                }
            }
            
            points[i].cluster = cluster_num;
            n_points[cluster_num]++;

            // Iterate over points to append data to centroids
            for (int j = 0; j < points_dimensions; ++j) {
                sum[cluster_num][j] += points[i].x[j];
            }
        }
                
        boolean = 1;
    
        // Compute new centroids
        #pragma omp parallel for firstprivate(k, points_dimensions)
        for (int i = 0; i < k; ++i) {
            int n_p = n_points[i];
            n_points[i] = 0;

            for (int j = 0; j < points_dimensions; j++) {
                double prov_sum = sum[i][j]/n_p;
                sum[i][j] = 0.0;
                if (centroids[i].x[j] != prov_sum) { 
                    #pragma omp critical
                    {
                        boolean = 1;
                        centroids[i].x[j] = prov_sum;
                    }
                }    
            }
        }
    }

    
    //Freeing points, sum and centroids
    for (int i = 0; i < k; ++i) {
        free(sum[i]);
        delete_point(&centroids[i]);
    }
    free(n_points);
    free(sum);
    free(centroids);
}
