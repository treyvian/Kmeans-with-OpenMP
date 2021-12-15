#ifndef POINT_H

#define POINT_H

typedef struct point {
    double *x;
    int dimensions, cluster;
    double minDist;
} point;

void point_init (point*, double*, int);

double distance (point, point);

void copy_point (point *, point *);

int equals (point *, point *);

void print_point (point *);

void delete_x(point *);

#endif