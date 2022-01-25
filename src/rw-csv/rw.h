#ifndef RW_H

#define RW_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <assert.h>

#include "../point/point.h"

/*
* Read a filename.csv and saves the data extracted in to the * data array
*
* @param row number of rows that need to be read
* @param col number of columns that need to be read
* @param filename name of the file that needs to be read
* @param data array in which to save the data read
*/
void read_csv (int row, int col, const char *filename, double **data);

/*
* Output a csv file with the points in input. 
*
* @param points data to write on the file
* @param n dimension of the points array
* @param name name to give to the file in ouput
* @param header header of the csv file
*/
void create_marks_csv(point *points, int n, const char *name, const char *header);

#endif