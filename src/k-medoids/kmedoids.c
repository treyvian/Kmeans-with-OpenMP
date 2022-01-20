#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

#include "kmedoids.h"


void k_medoids (point *points, 
                        const int n, 
                        const int k) {

    assert(points != NULL);

    point *medoids = (point *)malloc(k * sizeof(point));
    assert(medoids != NULL);

    point *best_medoids = (point *)malloc(k * sizeof(point));
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
            copy_point(&points[random], &medoids[i]);
        }
    }

    double total_cost = __DBL_MAX__;

    double distance, new_total_cost;


    for (int i=0; i<k; ++i) {
        copy_point(&medoids[i], &best_medoids[i]);
    }

    for (int i = 0; i<k; ++i){
        for (int j = 0; j<n; ++j){
            if (!equals(&points[j], &medoids[i])) {            
                new_total_cost = 0;
                distance = 0;
                copy_point(&points[j], &medoids[i]);
        
                for (int t = 0; t < n; ++t) {
                    for (int r = 0; r < k; ++r) {
                        distance = manhattan_distance(&medoids[r], &points[t]);

                        if (points[t].min_distance > distance) {
                            points[t].min_distance = distance; 
                        }
                    }

                    new_total_cost += points[t].min_distance; 
                    points[t].min_distance = __DBL_MAX__;
                }

                if (total_cost > new_total_cost) {
                    total_cost = new_total_cost;
                    copy_point(&points[j], &best_medoids[i]);
                } else {
                    copy_point(&best_medoids[i], &medoids[i]);
                }
            }                      
        }
        copy_point(&best_medoids[i], &medoids[i]);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            distance = manhattan_distance(&best_medoids[j], &points[i]);

            if (distance < points[i].min_distance){
                points[i].cluster = j;
                points[i].min_distance = distance;
            }
        }
           
        points[i].min_distance = __DBL_MAX__;
    }
    
    for (int j = 0; j < k; ++j) {
        print_point(&best_medoids[j]);
    }
    printf("Best cost:%f\n",total_cost);

    // TODO: check valgrind error
    for (int i = 0; i < k; ++i) {
        delete_x(&best_medoids[i]);
        delete_x(&medoids[i]);
    }
    free(best_medoids);
    free(medoids);
}