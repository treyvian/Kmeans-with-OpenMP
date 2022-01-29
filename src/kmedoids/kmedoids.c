#include "kmedoids.h"

void k_medoids (double **points,
                int *clusters,
                const int n, 
                const int dimensions,
                const int k) {

    assert(points != NULL);
    assert(clusters != NULL);

    double **medoids = (double **)calloc(k, sizeof(double *));
    assert(medoids != NULL);

    double **best_medoids = (double **)calloc(k, sizeof(double*));
    assert(best_medoids != NULL);

    for (int i = 0; i < k; i++) {
        medoids[i] = (double *)calloc(dimensions, sizeof(double));
        assert(medoids[i]);
        best_medoids[i] = (double *)calloc(dimensions, sizeof(double));
        assert(best_medoids[i]);
        
        medoids[i] = points[i];
        best_medoids[i] = medoids[i];
    }

    double total_cost = __DBL_MAX__;

    double distance, p_distance, new_total_cost;
    #pragma omp parallel for collapse(2) private(distance, p_distance, new_total_cost) firstprivate(total_cost, k, n, dimensions, medoids)
    for (int i = 0; i<n; ++i){
        for (int j = 0; j<k; ++j){
            if (points[i] != medoids[j]) {  
                    
                new_total_cost = 0;
                medoids[j] = points[i];

                for (int t = 0; t < n; ++t) {
                    p_distance = __DBL_MAX__;
                    for (int r = 0; r < k; ++r) {

                        distance = manhattan_distance(medoids[r], points[t], dimensions);
                    
                        if (p_distance > distance) {
                            p_distance = distance;
                        }
                    } 
                    
                    new_total_cost += p_distance;
                }
                
                #pragma omp critical
                {
                    if (total_cost > new_total_cost) {
                        total_cost = new_total_cost;
                        best_medoids[j] = points[i];
                    } else {
                        medoids[j] = best_medoids[j];
                    } 
                }   
            }                    
        }
    }

    int clust_num;

    #pragma omp parallel for private(distance, p_distance, clust_num) schedule(static)
    for (int i = 0; i < n; ++i) {
        p_distance = __DBL_MAX__;
        for (int j = 0; j < k; ++j) {
            distance = manhattan_distance(best_medoids[j], points[i], dimensions);

            if (distance < p_distance){
                p_distance = distance;
                clust_num = j;
            }
        }
        
        clusters[i] = clust_num;
    }

    free(best_medoids);
    free(medoids);
}
