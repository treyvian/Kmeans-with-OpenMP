#include <stdio.h>
#include <stdlib.h>

#include "point.h"

void point_init (point *p, double *x, int dimensions) {
    
    if (p == NULL){
        perror("null pointer to the init function\n");   
        exit(-1);
    }

    p->x = x;
    p->dimensions = dimensions;
    p->cluster = -1;
    p->minDist = __DBL_MAX__;
}

double distance (point *p1, point *p2) {

    if (p1 == NULL || p2 == NULL) {
        perror("null pointer in distance method\n");
        exit(-1);
    }

    double dist = 0;

    for (int i = 0; i < p1->dimensions; ++i) {
        dist += (p2->x[i] - p1->x[i]) * (p2->x[i] - p1->x[i]);
    }
    
    return dist;
}

void copy_point (point *o, point *p) {
    if (o == NULL || p == NULL) {
        perror("null pointer in copy method\n");
        exit(-1);
    }

    p->x = (double *)malloc(o->dimensions * sizeof(double));
    for (int i = 0; i < o->dimensions; ++i) {
        p->x[i] = o->x[i];
    }
    p->dimensions = o->dimensions;
    p->cluster = o->cluster;
    p->minDist = o->minDist;
}


void reset_point (point *p) {
    if (p == NULL) {
        perror("null pointer in reset method\n");
        exit(-1);
    }

    p->cluster = -1;
    p->minDist = __DBL_MAX__;
}


int equals (point *p1, point *p2){
    if (p1 == NULL || p2 == NULL) {
        perror("one of the pointers in input is null\n");
        exit(-1);
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

void delete_x (point *p) {
    if (p == NULL) {
        perror("null pointer in delete method\n");
        return;
    }

    free(p -> x);
}
