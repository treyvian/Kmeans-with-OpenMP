#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

#include <time.h> /* for time() */
#include <omp.h>

/*
* My methods
*/
#include "../kmedoids/kmedoids.h"
#include "../kmeans/kmeans.h"
#include "../silhouette-score/silhouette.h"
#include "../rw-csv/rw.h"


int main (int argc, char const *argv[]) {

    // Check for the minimum number of input parameters
    assert(argc >= 3);

    int row = atoi(argv[1]);
	int col = atoi(argv[2]);
	char fname[256];	
    strcpy(fname, argv[3]);

    int dimensions = 3;

    // Support array for storing the data from the csv file
    double **supp_array = (double **)malloc(row * sizeof(double *));
	for (int i = 0; i < row; ++i){
		supp_array[i] = (double *)malloc(col * sizeof(double));
	}

    double **data = (double **)malloc((row - 1) * sizeof(double *));
    assert(data != NULL);

    // Reading the csv file
	read_csv(row, col, fname, supp_array);

    for (int i = 1; i < row; ++i) {
        data[i-1] = (double *)malloc(dimensions * sizeof(double));
        assert(data[i-1] != NULL);

        data[i-1][0] = (double) supp_array[i][0];
        data[i-1][1] = (double) supp_array[i][1];
        data[i-1][2] = (double) supp_array[i][2];
    } 

    int *clusters = (int *)malloc((row - 1) * sizeof(int));
    assert(clusters != NULL);

    const int data_size = row-1;

    /*
    * K-means
    */
    int max_iterations = 500;
    double tstart, elapsed;

    // Starting the timer for kmeans
    tstart = omp_get_wtime();
    k_means(data, clusters, data_size, dimensions, max_iterations, 6);
    
    // Stopping the timer for kmeans
    elapsed = omp_get_wtime() - tstart;
    printf("Time kmeans %f ", elapsed);

    // //Starting the timer for silhouette score
    tstart = omp_get_wtime();
    silhouette_score(data, clusters, data_size, dimensions, 6);

    // Stopping the timer for silhouette score
    elapsed = omp_get_wtime() - tstart;
    printf("%f\n", elapsed);

    // Header of the csv file in output
    char *header = "MedInc,Latitude,Longitude,Cluster\n";
    
    // Name of the csv file in output
    char *filename = "output_house_kmeans.csv";

    // Creating the file in output
    create_marks_csv(data, clusters, data_size, dimensions, filename, header);    

    /*
    * K-medoids
    */
    // Starting the timer for kmedoids
    tstart = omp_get_wtime();
    k_medoids(data, clusters, data_size, dimensions, 6);

    // Stopping the timer for kmedoids
    elapsed = omp_get_wtime() - tstart;
    printf("Time kmedoids %f ", elapsed);
    // Starting the timer for silhouette score
    tstart = omp_get_wtime();
    double sil = silhouette_score(data, clusters, data_size, dimensions, 6);

    // Stopping the timer for silhouette score
    elapsed = omp_get_wtime() - tstart;
    printf(" %f ", sil);
    printf("%f\n", elapsed);

    header = "MedInc,Latitude,Longitude,Cluster\n";
    filename = "output_housing_kmedoids.csv";
    
    // Creating the file in output
    create_marks_csv(data, clusters, data_size, dimensions, filename, header);

    
    // Freeing memory for the array data
    for (int i=0; i < row; i++) {
        free(supp_array[i]);
    }
    free(supp_array);
    free(data);
    free(clusters);
    
    return 0;
}