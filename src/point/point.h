#ifndef POINT_H

#define POINT_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <omp.h>

/*
* Struct that represent a point in the dataset
*/
#pragma pack(1)
typedef struct point {
    double *x; /**< coordinates of the point */
    int dimensions; /**< dimensions of the point  */
    int cluster; /**< cluster number assigned */
    double min_distance; /**< distance from the closest centeroid/metroids */
} point;

/*
* Initiate a point with the parameters in input
* 
* @param p the point which will be initialized
* @param x the coordinates of the point
* @param dimensions dimensions of the point
*/
void point_init (point *p, double *x, int dimensions);

/*
* Calculate the euclidian distance between 2 points
*
* @param p1 first point to take in consideration
* @param p2 second point to take in consideration
* @return the distance between p1 and p2
*/
double euclidian_distance (point *p1, point *p2);

/*
* Calculate the manhattan distance between 2 points
*
* @param p1 first point to take in consideration
* @param p2 second point to take in consideration
* @return the distance between p1 and p2
*/
double manhattan_distance (point *p1, point *p2);

/*
* Copy a point to another
*
* @param dst destination of the copy process
* @param src source point that needs to be copied
*/
void copy_point (point *dst, const point *src);

/*
* Reset the struct point to the initial values
*
* @param point that needs to be resetted to inital values
*/
void reset_point (point *p);

/*
* Check if two points have the same coordinates
*
* @param p1 first point to compare
* @param p2 second point to compare
* @return 1 if they have the same coordinates 0 otherwise
*/
int equals (const point *p1, const point *p2);

/*
* Print the point coordinates
*
* @param p point that needs to be printed
*/
void print_point (const point *p);

/*
* Free the coordinates array inside the struct
*
* @param free the coordinate array x
*/
void delete_point(point *);

#endif