#include <stdio.h>
#include <stdlib.h>
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
    assert(data != NULL);

    double *supp_vector;
    int dimensions = 2;

    // Initialize the data array with one point for each row
    for (int i = 1; i < row; ++i) {
        supp_vector = (double *)malloc(dimensions * sizeof(double)); 
        assert(supp_vector != NULL);

        supp_vector[0] = (double) dat[i][3];
        supp_vector[1] = (double) dat[i][4];
        point_init(&data[i-1], supp_vector, dimensions);
    }

    // Freeing memory for the array dat
    for (int i=0; i < row; i++) {
        free(dat[i]);
    }
    free(dat);

    const size_t data_size = row-1;

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
        k_means(data, data_size, max_iterations, i);
        sil_score = silhouette_score(data, data_size, i);
        printf("%f ", sil_score);
        if (best_silhouette < sil_score) {
            best_silhouette = sil_score;
            best_cluster = i;
        }
    }

    assert(best_cluster != -1);
    
    double tstart, elapsed;

    // Starting the timer for kmeans
    tstart = omp_get_wtime();
    k_means(data, data_size, max_iterations, best_cluster);
    
    // Stopping the timer for kmeans
    elapsed = omp_get_wtime() - tstart;
    printf("Time kmeans %f ", elapsed);

    // //Starting the timer for silhouette score
    tstart = omp_get_wtime();
    silhouette_score(data, data_size, best_cluster);

    // Stopping the timer for silhouette score
    elapsed = omp_get_wtime() - tstart;
    printf("%f\n", elapsed);


    // Create output csv file for kmeans
    const char *header = "X,Y,Cluster\n";
    const char *filename = "output_mall_kmeans.csv";
    
    create_marks_csv(data, data_size, filename, header);

    /*
    * K-medoids
    */

    best_silhouette = -1;
    best_cluster = -1;

    for (int i = 2; i < (n_clusters + 1); i++) {
        k_medoids(data, data_size, i);
        sil_score = silhouette_score(data, data_size, i);
        
        if (best_silhouette < sil_score) {
            best_silhouette = sil_score;
            best_cluster = i;
        }
    }

    assert(best_cluster != -1);
    
    // Starting the timer for kmedoids
    tstart = omp_get_wtime();
    k_medoids(data, data_size, best_cluster);

    // Stopping the timer for kmedoids
    elapsed = omp_get_wtime() - tstart;
    printf("Time kmedoids %f ", elapsed);
    // Starting the timer for silhouette score
    tstart = omp_get_wtime();
    silhouette_score(data, data_size, best_cluster);

    // Stopping the timer for silhouette score
    elapsed = omp_get_wtime() - tstart;
    printf("%f\n", elapsed);

    // Create output csv file for kmedoids
    const char *header_p = "X,Y,Cluster\n";
    const char *filename_p = "output_mall_kmedoids.csv";
    
    // Creating the file in output
    create_marks_csv(data, data_size, filename_p, header_p);
    
    // Freeing memory for the array data
    for (int i = 0; i < data_size; ++i) {
        delete_point(&data[i]);
    }
    free(data);

    return 0;
}