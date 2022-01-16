#ifndef POINT_H

#define POINT_H

typedef struct point {
    double *x;
    int dimensions, cluster;
    double min_distance;
} point;

void point_init (point*, double*, int);

double euclidian_distance (point *, point *);

double manhattan_distance (point *, point *);

void copy_point (point *, point *);

void reset_point (point *);

int equals (point *, point *);

void print_point (point *);

void delete_x(point *);

#endif