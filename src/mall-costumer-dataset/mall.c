#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
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
    
    assert(argc >= 3);

	int row = atoi(argv[1]);
	int col = atoi(argv[2]);
	char fname[256];	
    strcpy(fname, argv[3]);

    int dimensions = 2; /* dimensions of the points */

    // Support array for storing the data from the csv file
    double **supp_array = (double **)malloc(row * sizeof(double *));
    assert(supp_array != NULL);
	for (int i = 0; i < row; ++i){
		supp_array[i] = (double *)malloc(col * sizeof(double));
        assert(supp_array[i] != NULL);
	}

    double **data = (double **)malloc((row - 1) * sizeof(double *));
    assert(data != NULL);

    // Reading the csv file
	read_csv(row, col, fname, supp_array);

    /*
    * After the supp_array is filled with the values from the csv in input
    * stores in the data array only the columns that needs to be clustered
    */
    for (int i = 1; i < row; ++i) {
        data[i-1] = (double *)malloc(dimensions * sizeof(double));
        assert(data[i-1] != NULL);

        data[i-1][0] = (double) supp_array[i][3];
        data[i-1][1] = (double) supp_array[i][4];
    } 

    int *clusters = (int *)malloc((row - 1) * sizeof(int));
    assert(clusters != NULL);

    /* Number of rows in the data_size dataset */
    const int data_size = row-1;

    int n_clusters = atoi(argv[4]);
    assert(n_clusters > 1);

    /*
    * K-means
    */
    int max_iterations = 100;
    double best_silhouette = -1;
    double sil_score;
    int best_cluster = -1;
    
    for (int i = 2; i < (n_clusters + 1); i++) {
        k_means(data, clusters, data_size, dimensions, max_iterations, i);
        sil_score = silhouette_score(data, clusters, data_size, dimensions, i);
        if (best_silhouette < sil_score) {
            best_silhouette = sil_score;
            best_cluster = i;
        }
    }

    assert(best_cluster != -1);
    
    double tstart, elapsed;

    // Starting the timer for kmeans
    tstart = omp_get_wtime();
    k_means(data, clusters, data_size, dimensions, max_iterations, best_cluster);
    
    // Stopping the timer for kmeans
    elapsed = omp_get_wtime() - tstart;
    printf("Time kmeans %f \n", elapsed);

    // //Starting the timer for silhouette score
    tstart = omp_get_wtime();
    silhouette_score(data, clusters, data_size, dimensions, best_cluster);

    // Stopping the timer for silhouette score
    elapsed = omp_get_wtime() - tstart;
    printf("Time silhouette %f \n", elapsed);


    // Create output csv file for kmeans
    char header[128];
    char filename[128];
    strcpy(header, "X,Y,Cluster\n");
    strcpy(filename, "output_mall_kmeans.csv");
    
    create_marks_csv(data, clusters, data_size, dimensions, filename, header);

    /*
    * K-medoids
    */

    best_silhouette = -1;
    best_cluster = -1;

    for (int i = 2; i < (n_clusters + 1); i++) {
        k_medoids(data, clusters, data_size, dimensions, i);
        sil_score = silhouette_score(data, clusters, data_size, dimensions, i);
        if (best_silhouette < sil_score) {
            best_silhouette = sil_score;
            best_cluster = i;
        }
    }

    assert(best_cluster != -1);
    
    // Starting the timer for kmedoids
    tstart = omp_get_wtime();
    k_medoids(data, clusters, data_size, dimensions, best_cluster);

    // Stopping the timer for kmedoids
    elapsed = omp_get_wtime() - tstart;
    printf("Time kmedoids %f \n", elapsed);

    // Create output csv file for kmedoids
    strcpy(filename, "output_mall_kmedoids.csv");
    
    // Creating the file in output
    create_marks_csv(data, clusters, data_size, dimensions, filename, header);
    
    // Freeing memory for the array data
    for (int i=0; i < row; i++) {
        free(supp_array[i]);
        if (i != (row-1))
            free(data[i]);
    }
    free(supp_array);
    free(data);
    free(clusters);

    return 0;
}