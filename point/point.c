#include <stdio.h>
#include <math.h>

#include "point.h"

int point_init (point *p, double *x, int dimensions) {
    
    if (p == NULL)
        return -1;

    p->x = x;
    p->dimensions = dimensions;
    p->cluster = -1;
    p->minDist = __DBL_MAX__;
    
    return 0;
}

double distance (point p1, point p2) {
    double dist = 0;

    for (int i = 0; i < p1.dimensions; i++) {
        dist += (p2.x[i] - p1.x[i]) * (p2.x[i] - p1.x[i]);
    }
    
    return sqrt(dist);
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
