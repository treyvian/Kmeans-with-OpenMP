#include "kmedoids.h"

int point_dimensions;

void copy_point (double src[], double dst[]) {
    for (int i = 0; i < point_dimensions; i++){
        dst[i] = src[i];
    }
};

void k_medoids (double **points,
                int *clusters,
                const int n, 
                const int dimensions,
                const int k) {

    assert(points != NULL);
    assert(clusters != NULL);

    point_dimensions = dimensions;

    double medoids[k][dimensions];
    double best_medoids[k][dimensions];

    for (int i = 0; i < k; i++) {  
        for (int j = 0; j < k; j++){
            medoids[i][j] = points[i][j];
            best_medoids[i][j] = points[i][j];
        }
    }

    double total_cost = __DBL_MAX__;
    double new_total_cost, distance, p_distance;
    #pragma omp parallel firstprivate(n, k, dimensions)
    {
        #pragma omp for collapse(2) private(new_total_cost, distance, p_distance)
        for (int i = 0; i<n; ++i){
            for (int j = 0; j<k; ++j){   
                new_total_cost = 0;

                int t, r, h;    
                copy_point(points[i], medoids[j]);

                for (t = 0; t < n; ++t) {
                    p_distance = __DBL_MAX__;
                
                    for (r = 0; r < k; ++r) {
                        for (h = 0; h < dimensions; ++h) {
                            distance += abs(medoids[r][h] - points[t][h]);
                        } 

                        if (p_distance > distance) {
                            p_distance = distance;
                        }
                        distance = 0;
                    }

                    new_total_cost += p_distance;
                }

                #pragma omp critical
                {
                    if (total_cost > new_total_cost) {
                        total_cost = new_total_cost;
                        copy_point(points[i], medoids[j]);
                    } else {
                        copy_point(best_medoids[i], medoids[j]);   
                    } 
                }                 
            }
        }

        #pragma omp for private(distance, p_distance)
        for (int i = 0; i < n; ++i) {
            int clust_num;
            p_distance = __DBL_MAX__;

            int j,h;  
            for (j = 0; j < k; ++j) {
                for (h = 0; h < dimensions; ++h) {
                    distance += abs(best_medoids[j][h] - points[i][h]);
                } 

                if (distance < p_distance){
                    p_distance = distance;
                    clust_num = j; 
                }
                distance = 0;
            }
            #pragma omp crtitical
            {
                clusters[i] = clust_num;
            }
        }
    }
}
