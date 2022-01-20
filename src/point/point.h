#ifndef POINT_H

#define POINT_H

/*
* Struct that represent a point in the dataset
*/
typedef struct point {
    double *x; /**< coordinates of the point */
    int dimensions; /**< dimensions of the point  */
    int cluster; /**< cluster number assigned */
    double min_distance; /**< distance from the closest centeroid/metroids */
} point;

/*
* Initiate a point with the parameters in input
*/
void point_init (point*, double*, int);

/*
* Calculate the euclidian distance between 2 points
*/
double euclidian_distance (point *, point *);

/*
* Calculate the manhattan distance between 2 points
*/
double manhattan_distance (point *, point *);

/*
* Copy a point to another
*/
void copy_point (point *, const point *);

/*
* Reset the struct point to the initial values
*/
void reset_point (point *);

/*
* Check if two points have the same coordinates
*/
int equals (const point *, const point *);

/*
* Print the point coordinates
*/
void print_point (const point *);

/*
* Free the coordinates array inside the struct
*/
void delete_point(point *);

#endif