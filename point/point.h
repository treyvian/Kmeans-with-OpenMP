#ifndef POINT_H

#define POINT_H

typedef struct point {
    double *x;
    int dimensions, cluster;
    double minDist;
} point;

int point_init (point*, double*, int);
double distance (point, point);
void print_point (point *p);


#endif