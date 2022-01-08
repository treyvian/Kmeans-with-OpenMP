#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "../k-means/kmeans.h"
#include "../rw-csv/rw.h"

int main (int argc, char const *argv[]) {

    if (argc < 3) {
		printf("Please specify the CSV file as an input.\n");
		exit(1);
	}

    int row = atoi(argv[1]);
	int col = atoi(argv[2]);
	char fname[256];	
    strcpy(fname, argv[3]);

    double **dat;
	dat = (double **)malloc(row * sizeof(double *));
	for (int i = 0; i < row; ++i){
		dat[i] = (double *)malloc(col * sizeof(double));
	}

	read_csv(row, col, fname, dat);

    point *data = (point *)malloc((row - 1) * sizeof(point));

    double *supp_vector;
    int dimensions = 3;
    for (int i = 1; i < row; ++i) {
        supp_vector = (double *)malloc(dimensions * sizeof(double)); 
        supp_vector[0] = (double) dat[i][0];
        supp_vector[1] = (double) dat[i][6];
        supp_vector[2] = (double) dat[i][7];
        point_init(&data[i-1], supp_vector, dimensions);
    }

    for (int i=0; i<row; i++) {
        print_point(&data[i]);
    }

    return 0;
}