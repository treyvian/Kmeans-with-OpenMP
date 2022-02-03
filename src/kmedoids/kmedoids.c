#include "kmedoids.h"

/*
* Help method to calculate the manhattan distance
*/
double manhattan_distance (const double *p1, const double *p2, int dimensions) {

    double distance = 0;

    for (int i = 0; i < dimensions; ++i) {
        distance += abs(p1[i] - p2[i]);
    } 

    return distance;
}

/*
* Help method to copy a point coordinates from the source array
* to the destination one
*/
void copy_point (const double *src, double *dst, int dimensions) {
    for (int i = 0; i < dimensions; i++){
        dst[i] = src[i];
    }
}

void k_medoids (double **points,
                int *clusters,
                const int n, 
                const int dimensions,
                const int k) {

    // 5 arguments
    // data       - n x dimensions size array contains the data in input
    // clusters   - n-dimensional array that at the end of the algorithm will 
    //              containts the number of the cluster associated to the point
    // n          - number of points in input
    // dimensions - dimension of the point in input
    // k          - number of cluster to divide the dataset into

    assert(points != NULL);
    assert(clusters != NULL);

    double medoids[k][dimensions];

    /* used to store the best combination of points found */
    double best_medoids[k][dimensions]; 

    
    for (int i = 0; i < k; i++) {  
        copy_point(points[i], medoids[i], dimensions);
        copy_point(points[i], best_medoids[i], dimensions);
    }

    double total_cost = __DBL_MAX__; /* stores the min cost found */
    double new_total_cost, distance, p_distance;
    #pragma omp parallel firstprivate(n, k, dimensions)
    {
        /*
        * For each medoid m, and for each non-medoid data point o:
        * - Consider the swap of m and o, and compute the cost change
        * - If the cost change is the current best, remember this m and o 
        * combination by saving them in the best_medoids array
        */
        #pragma omp for collapse(2) private(new_total_cost, distance, p_distance) firstprivate(medoids)
        for (int i = 0; i<n; ++i){
            for (int j = 0; j<k; ++j){   
                new_total_cost = 0;
                
                copy_point(points[i], medoids[j], dimensions);
                
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

                if (total_cost > new_total_cost) {
                    #pragma omp critical
                    {
                        total_cost = new_total_cost;
                        copy_point(points[i], best_medoids[j], dimensions);
                    }
                } else {
                    copy_point(best_medoids[j], medoids[j], dimensions);   
                }              
            }
        }
        
        /*
        * Once the best combination is found recalculate the distance
        * and save the cluster number for each point
        */
        #pragma omp for private(distance, p_distance)
        for (int i = 0; i < n; ++i) {
            int num_clust;
            p_distance = __DBL_MAX__; 
            for (int j = 0; j < k; ++j) {
                distance = manhattan_distance(best_medoids[j], points[i], dimensions);

                if (distance < p_distance){
                    p_distance = distance;
                    num_clust = j; 
                }
            }
            #pragma omp critical
            {
            clusters[i] = num_clust; /* insert the right cluster number */
            }
        }
    }
}
