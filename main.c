#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "k-means.h"

int main (int argc, char const *argv[]) {

    // Reading the csv file
    if (argc < 3) {
		printf("Please specify the CSV file as an input.\n");
		exit(0);
	}

	int row     = atoi(argv[1]);
	int col     = atoi(argv[2]);
	char fname[256];	strcpy(fname, argv[3]);

	int **dat;
	dat = (int **)malloc(row * sizeof(int *));
	for (int i = 0; i < row; ++i){
		dat[i] = (int *)malloc(col * sizeof(int));
	}

	read_csv(row, col, fname, dat);

    

    point *data = (point *)malloc((row - 1) * sizeof(point));

    double *x;

    for (int i = 1; i < row; ++i) {
        x = (double *)malloc(2 * sizeof(double)); 
        x[0] = (double) dat[i][3];
        x[1] = (double) dat[i][4];
        point_init(&data[i-1], x, 2);
    }

    const size_t data_size = row-1;
     
    // for (int i = 0; i < data_size; i++) {
    //     print_point(&data[i]);
    // }

    // freeing memory for the array dat
    for (int i=0; i < data_size; i++) {
        free(dat[i]);
    }
    free(dat);
    
    kMeansClustering(data, data_size, 100, 5);

    char *name = "output.csv";

    create_marks_csv(name,data,data_size);

    

    
    // Freeing memory for the array data
    free(data);

    return 0;
}