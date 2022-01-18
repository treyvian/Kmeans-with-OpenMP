#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <time.h> /* for time() */
#include <omp.h>

/*
* Inclusion of my implementations
*/
#include "../../k-medoids/kmedoids.h"
#include "../../silhouette-score/silhouette.h"
#include "../../rw-csv/rw.h"

int main (int argc, char const *argv[]) {
    
    // Check for the minimum number of input parameters
    if (argc < 3) {
		printf("Please specify the CSV file as an input.\n");
		exit(1);
	}

	int row = atoi(argv[1]);
	int col = atoi(argv[2]);
	char fname[256];	
    strcpy(fname, argv[3]);

    // Support array for storing the data from the csv file
	double **dat;
	dat = (double **)malloc(row * sizeof(double *));
	for (int i = 0; i < row; ++i){
		dat[i] = (double *)malloc(col * sizeof(double));
	}

    // Reading the csv file
	read_csv(row, col, fname, dat); 

     // Bi-dimensional array for storing the the data to work with
    point *data = (point *)malloc((row - 1) * sizeof(point));

    double *supp_vector;
    int dimensions = 2;

    // Initialize the data array with one point for each row
    for (int i = 1; i < row; ++i) {
        supp_vector = (double *)malloc(dimensions * sizeof(double)); 
        supp_vector[0] = (double) dat[i][3];
        supp_vector[1] = (double) dat[i][4];
        point_init(&data[i-1], supp_vector, dimensions);
    }

    // Freeing support array dat
    for (int i=0; i < row; i++) {
        free(dat[i]);
    }
    free(dat);

    const size_t data_size = row-1;

    // Checking the validity of the range of clusters in input
    int n_clusters = atoi(argv[4]);
    if (n_clusters < 1) {
        printf("Number of clusters inserted not valid\n");
        exit(1);    
    }

    // KMeans implementations
    double tstart, elapsed;
    
    // Starting the timer for performance measurement
    tstart = omp_get_wtime();

    // Clustering the dataset
    k_medoids(data, data_size, 6);

    // Stopping the timer and print the result
    elapsed = omp_get_wtime() - tstart;
    printf("Elapsed time kmedoids %f\n", elapsed);

    // Starting the timer for performance measurement 
    tstart = omp_get_wtime();
    sil_score = silhouette_score(data, data_size, 6);

    // Stopping the timer and print the result
    elapsed = omp_get_wtime() - tstart;
    printf("Elapsed time silhouette %f\n", elapsed);

    // Header of the csv file in output
    const char *header = "MedInc,Latitude,Longitude,Cluster\n";

    // Name of the csv file in output
    const char *filename = "output_housing_kmedoids.csv";
    
    // Creating the file in output
    create_marks_csv(data, data_size, filename, header);
    
    // Freeing memory for the array data
    for (int i = 0; i < data_size; ++i) {
        delete_x(&data[i]);
    }
    free(data);

    return 0;
}