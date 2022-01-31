#include "kmedoids.h"

int point_dim;


double manhattan_distance (const double *p1, const double *p2) {

    double distance = 0;

    for (int i = 0; i < point_dim; ++i) {
        distance += abs(p1[i] - p2[i]);
    } 

    return distance;
}

void copy_point (const double *src, double *dst) {
    for (int i = 0; i < point_dim; i++){
        dst[i] = src[i];
    }
}

void k_medoids (double **points,
                int *clusters,
                const int n, 
                const int dimensions,
                const int k) {

    assert(points != NULL);
    assert(clusters != NULL);

    point_dim = dimensions;

    double medoids[k][dimensions];
    double best_medoids[k][dimensions];

    
    for (int i = 0; i < k; i++) {  
        copy_point(points[i], medoids[i]);
        copy_point(points[i], best_medoids[i]);
    }

    double total_cost = __DBL_MAX__;
    double new_total_cost, distance, p_distance;
    #pragma omp parallel firstprivate(n, k, dimensions)
    {
        #pragma omp for collapse(2) private(new_total_cost, distance, p_distance) firstprivate(medoids)
        for (int i = 0; i<n; ++i){
            for (int j = 0; j<k; ++j){   
                new_total_cost = 0;
                
                copy_point(points[i], medoids[j]);
                
                for (int t = 0; t < n; ++t) {
                    p_distance = __DBL_MAX__;
                
                    for (int r = 0; r < k; ++r) {
                        distance = manhattan_distance(medoids[r], points[t]);

                        if (p_distance > distance) {
                            p_distance = distance;
                        }
                    }

                    new_total_cost += p_distance;
                }

                if (total_cost > new_total_cost) {
                    #pragma omp critical
                    {
                        total_cost = new_total_cost;
                        copy_point(points[i], best_medoids[j]);
                    }
                } else {
                    copy_point(best_medoids[j], medoids[j]);   
                }              
            }
        }
        
        #pragma omp for private(distance, p_distance)
        for (int i = 0; i < n; ++i) {
            int num_clust;
            p_distance = __DBL_MAX__; 
            for (int j = 0; j < k; ++j) {
                distance = manhattan_distance(best_medoids[j], points[i]);

                if (distance < p_distance){
                    p_distance = distance;
                    num_clust = j; 
                }
            }
            #pragma omp critical
            {
            clusters[i] = num_clust;
            }
        }
    }
}
