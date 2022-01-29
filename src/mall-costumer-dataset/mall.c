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

    int dimensions = 2;

    // Support array for storing the data from the csv file
    double **supp_array = (double **)malloc(row * sizeof(double *));
	for (int i = 0; i < row; ++i){
		supp_array[i] = (double *)malloc(col * sizeof(double));
	}

    double **data = (double **)calloc((row - 1), sizeof(double *));
    assert(data != NULL);

    // Reading the csv file
	read_csv(row, col, fname, supp_array);

    for (int i = 1; i < row; ++i) {
        data[i-1] = (double *)calloc(dimensions, sizeof(double));
        assert(data[i-1] != NULL);

        data[i-1][0] = (double) supp_array[i][3];
        data[i-1][1] = (double) supp_array[i][4];
    } 

    int *clusters = (int *)calloc((row - 1), sizeof(int));
    assert(clusters != NULL);

    const int data_size = row-1;

    int n_clusters = atoi(argv[4]);
    assert(n_clusters > 1);

    /*
    * K-means
    */
    int max_iterations = 500;
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
    printf("Time kmeans %f ", elapsed);

    // //Starting the timer for silhouette score
    tstart = omp_get_wtime();
    silhouette_score(data, clusters, data_size, dimensions, best_cluster);

    // Stopping the timer for silhouette score
    elapsed = omp_get_wtime() - tstart;
    printf("%f\n", elapsed);


    // Create output csv file for kmeans
    char *header = "X,Y,Cluster\n";
    char *filename = "output_mall_kmeans.csv";
    
    create_marks_csv(data, clusters, data_size, dimensions, filename, header);

    /*
    * K-medoids
    */

    best_silhouette = -1;
    best_cluster = -1;

    for (int i = 2; i < (n_clusters + 1); i++) {
        k_medoids(data, clusters, data_size, dimensions, i);
        sil_score = silhouette_score(data, clusters, data_size, dimensions, i);
        printf(" %.3f ", sil_score);
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
    printf("Time kmedoids %f ", elapsed);
    // Starting the timer for silhouette score
    tstart = omp_get_wtime();
    silhouette_score(data, clusters, data_size, dimensions, best_cluster);

    // Stopping the timer for silhouette score
    elapsed = omp_get_wtime() - tstart;
    printf("%f\n", elapsed);

    // Create output csv file for kmedoids
    header = "X,Y,Cluster\n";
    filename = "output_mall_kmedoids.csv";
    
    // Creating the file in output
    create_marks_csv(data, clusters, data_size, dimensions, filename, header);
    
    // Freeing memory for the array data
        // Freeing memory for the array dat
    for (int i=0; i < row; i++) {
        free(supp_array[i]);
    }
    free(supp_array);
    free(data);
    // free(clusters);

    return 0;
}