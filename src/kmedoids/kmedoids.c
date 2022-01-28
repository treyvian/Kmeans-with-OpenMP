#include "kmedoids.h"


void k_medoids (point *points, 
                        const int n, 
                        const int k) {

    assert(points != NULL);

    point *medoids = (point *)calloc(k, sizeof(point));
    assert(medoids != NULL);

    point *best_medoids = (point *)calloc(k, sizeof(point));
    assert(best_medoids != NULL);

    int random, iter, boolean;
    int points_dimensions = points->dimensions;

    srand(time(0));
    for (int i = 0; i < k; i++) {
        boolean = 1;
        random = rand() % n;
        for (int j = 0; j < i; j++) {
            if (equals(&points[random], &medoids[j])) {
                boolean = 0;
                i--;
                break;
            }    
        }
        if (boolean) {
            copy_point(&medoids[i], &points[random]);
        }
    }

    double total_cost = __DBL_MAX__;

    for (int i=0; i<k; ++i) {
        copy_point(&best_medoids[i], &medoids[i]);
    }
    
    double distance, p_distance;
    int clust_num;
    for (int i = 0; i<k; ++i){
        for (int j = 0; j<n; ++j){
            if (!equals(&points[j], &medoids[i])) {  
                         
                double new_total_cost = 0;
                copy_point(&medoids[i], &points[j]);

                #pragma omp parallel for reduction(+:new_total_cost) private(distance, p_distance) firstprivate(medoids)
                for (int t = 0; t < n; ++t) {
                    p_distance = __DBL_MAX__;
                    for (int r = 0; r < k; ++r) {
                        distance = manhattan_distance(&medoids[r], &points[t]);

                        if (p_distance > distance) {
                            p_distance = distance;
                        }
                    } 
                    
                    new_total_cost += p_distance;
                }
                
                if (total_cost > new_total_cost) {
                    total_cost = new_total_cost;
                    copy_point(&best_medoids[i], &points[j]);
                } else {
                    copy_point(&medoids[i], &best_medoids[i]);
                }    
            }                      
        }
        copy_point(&medoids[i], &best_medoids[i]);
    }

    #pragma omp parallel for private(distance, p_distance, clust_num)
    for (int i = 0; i < n; ++i) {
        p_distance = __DBL_MAX__;
        for (int j = 0; j < k; ++j) {
            distance = manhattan_distance(&best_medoids[j], &points[i]);

            if (distance < p_distance){
                p_distance = distance;
                clust_num = j;
            }
        }
        points[i].cluster = clust_num;
    }

    for (int i = 0; i < k; ++i) {
        delete_point(&best_medoids[i]);
        delete_point(&medoids[i]);
    }
    free(best_medoids);
    free(medoids);
}
