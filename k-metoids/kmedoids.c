#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "kmedoids.h"


void k_medoids (point *points, 
                        const int n, 
                        const int k) {

    if (points == NULL){
        perror("Missing input data");
        exit(1);
    }

    if (epochs <= 0) {
        perror("Number of epochs not correct");
        exit(1);
    }

    point *medoids = (point *)malloc(k * sizeof(point));

    /*TODO:check bool*/
    int bool, random, iter;
    int points_dimensions = points->dimensions;

    srand(time(0));
    for (int i = 0; i < k; i++) {
        bool = 1;
        random = rand() % n;
        for (int j = 0; j < i; j++) {
            if (equals(&points[random], &medoids[j])) {
                bool = 0;
                i--;
                break;
            }    
        }
        if (bool) {
            copy_point(&points[random], &metoids[i]);
        }
    }

    double total_cost = 0;

    double distance;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            distance = manhattan_distance(&medoids[j], &points[i]);

            if (distance < points[i].min_distance){
                points[i].min_distance = distance;
            }
        }

        total_cost += point[i].min_distance;    
    }

    double new_total_cost;
    int bool = 1;
    
    point *best_medoids = (point *)malloc(k * sizeof(point));

    for (int i = 0; i<k; ++i){
        copy_point(&medoids[i], &best_medoids[i])
    }

    for (int i = 0; i<k; ++i){
        for (int j = 0; j<n; ++j){
            new_total_cost = 0;
            copy_point(&point[j], &medoids[i]);

            double distance;
            for (int t = 0; t < n; ++t) {
                for (int r = 0; r < k; ++r) {
                    distance = manhattan_distance(&centroids[r], &points[t]);

                    if (distance < points[t].min_distance){
                        points[t].min_distance = distance; 
                    }
                }

                new_total_cost += point[t].min_distance;    
            }

            if (total_cost > new_total_cost) {
                total_cost = new_total_cost;
                copy_point(&point[j], &best_medoids[i]);
            }                     
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            dist = euclidian_distance(&best_medoids[j], &points[i]);

            if (dist < points[i].min_distance){
                points[i].cluster = j;
            }
        }
           
        points[i].min_distance = __DBL_MAX__;
    }

    for (int i = 0; i < k; ++i) {
        delete_x(&best_medoids[i]);
        delete_x(&medoids[i]);
    }
    free(best_medoids);
    free(medoids);
}