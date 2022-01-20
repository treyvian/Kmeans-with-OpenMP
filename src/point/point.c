#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "point.h"

void point_init (point *p, double *x, int dimensions) {

    assert(p != NULL || x != NULL);

    p->x = x;
    p->dimensions = dimensions;
    p->cluster = -1;
    p->min_distance = __DBL_MAX__;
}

double euclidian_distance (point *p1, point *p2) {

    assert(p1 != NULL || p2 != NULL);

    double distance = 0;

    for (int i = 0; i < p1->dimensions; ++i) {
        distance += (p2->x[i] - p1->x[i]) * (p2->x[i] - p1->x[i]);
    }
    
    return sqrt(distance);
}

double manhattan_distance (point *p1, point *p2) {
    assert(p1 != NULL || p2 != NULL);

    double distance = 0;

    for (int i = 0; i < p1->dimensions; ++i) {
        distance += abs(p2->x[i] - p1->x[i]);
    } 

    return distance;
}

void copy_point (point *o, point *p) {
    assert(o != NULL && p != NULL);

    if (p->x == NULL);

    p->x = (double *)malloc(o->dimensions * sizeof(double));
    assert(p->x != NULL); 

    for (int i = 0; i < o->dimensions; ++i) {
        p->x[i] = o->x[i];
    }
    p->dimensions = o->dimensions;
    p->cluster = o->cluster;
    p->min_distance = o->min_distance;
}


void reset_point (point *p) {
    assert(p != NULL);

    p->cluster = -1;
    p->min_distance = __DBL_MAX__;
}


int equals (const point *p1, const point *p2){

    assert(p1 != NULL && p2 != NULL);

    const point* d1 = p1;
    const point* d2 = p2;
    double diff = 0.0;

    for (int i = 0; i < p1->dimensions; ++i) {
        diff += d1->x[i] - d2->x[i]; 
        if (diff != 0) {
            return 0;
        }
    }
    return 1;
}

void print_point (const point *p) {

    if(p == NULL){
        printf("Pointer is null");
        return;
    }    
    
    printf("(");
    
    for (int i=0; i < p->dimensions; i++) {
        printf("%f ", p->x[i]);
    }

    printf(")\n");
}

void delete_x (point *p) {
    if (p == NULL) {
        return;
    }

    free(p -> x);
}
