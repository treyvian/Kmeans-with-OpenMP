#include <stdio.h>
#include <stdlib.h>

#include "point.h"

void point_init (point *p, double *x, int dimensions) {
    
    if (p == NULL){
        printf("null pointer to the init function\n");   
        return;
    }

    p->x = x;
    p->dimensions = dimensions;
    p->cluster = -1;
    p->minDist = __DBL_MAX__;
}

double distance (point p1, point p2) {
    double dist = 0;

    for (int i = 0; i < p1.dimensions; i++) {
        dist += (p2.x[i] - p1.x[i]) * (p2.x[i] - p1.x[i]);
    }
    
    return dist;
}

void copy_point (point *o, point *p) {
    if (o == NULL || p == NULL) {
        printf("null pointer in copy method\n");
        return;
    }

    p->x = (double *)malloc(o->dimensions * sizeof(double));
    for (int i = 0; i < o->dimensions; ++i) {
        p->x[i] = o->x[i];
    }
    p->dimensions = o->dimensions;
    p->cluster = o->cluster;
    p->minDist = o->minDist;
}

int equals (point *p1, point *p2){
    if (p1 == NULL || p2 == NULL) {
        printf("one of the pointers in input is null\n");
        return 0;
    }

    if (p1 == p2) {
        return 1;
    }

    if (p1->dimensions != p2-> dimensions) {
        return 0;
    }
    for (int i = 0; i < p1->dimensions; ++i) {
        if (p1->x[i] != p2->x[i]) {
            return 0;
        }
    }
    return 1;
}

void print_point (point *p) {

    if(p == NULL){
        printf("Pointer is null");
        
    } else {

        printf("(");
    
        for (int i=0; i < p->dimensions; i++) {
            printf("%f ", p->x[i]);
        }

        printf(")\n");
    }
}