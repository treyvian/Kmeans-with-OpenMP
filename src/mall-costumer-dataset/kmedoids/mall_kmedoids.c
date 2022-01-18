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
    
    if (argc == 3) {
		perror("Please specify the CSV file as an input.\n");
		exit(1);
	}

	int row = atoi(argv[1]);
	int col = atoi(argv[2]);
	char fname[256];	
    strcpy(fname, argv[3]);

	double **dat;
	
    dat = (double **)malloc(row * sizeof(double *));
    if (!dat) {
        perror("Heap allocation in mall_kmedoids not successful");
        exit(1);
    }

	for (int i = 0; i < row; ++i){
		dat[i] = (double *)malloc(col * sizeof(double));
        if (!dat[i]) {
            perror("Heap allocation in mall_kmedoids not successful");
            exit(1);
        }
	}

	read_csv(row, col, fname, dat); 

    point *data = (point *)malloc((row - 1) * sizeof(point));
    if (!data) {
        perror("Heap allocation of data in mall_kmedoids not successful");
        exit(1);
    }

    double *supp_vector;
    int dimensions = 2;

    for (int i = 1; i < row; ++i) {
        supp_vector = (double *)malloc(dimensions * sizeof(double));
        if (!supp_vector) {
            perror("Heap allocation of supp_vector in mall_kmedoids not successful");
            exit(1);
        }

        supp_vector[0] = (double) dat[i][3];
        supp_vector[1] = (double) dat[i][4];
        point_init(&data[i-1], supp_vector, dimensions);
    }


    // freeing memory for the array dat
    for (int i=0; i < row; i++) {
        free(dat[i]);
    }
    free(dat);

    const size_t data_size = row-1;

    int n_clusters = atoi(argv[4]);
    if (n_clusters < 1) {
        printf("Number of clusters inserted not valid\n");
        exit(1);    
    }

    // KMeans implementations
    int max_iterations = 500;
    double best_silhouette = -1;
    double sil_score;
    int best_cluster = -1;

    for (int i = 2; i < (n_clusters + 1); i++) {
        k_medoids(data, data_size, i);
        sil_score = silhouette_score(data, data_size, i);
        printf("with a silhouette score of %.3f \n", sil_score);
        if (best_silhouette < sil_score) {
            best_silhouette = sil_score;
            best_cluster = i;
        }
    }

    if (best_cluster == -1) {
        perror("Correct number of clusters not found\n");
        exit(1);
    }

    double tstart, elapsed;
    
    printf("Best number of clusters: %d, with silhouette score of: %f \n", best_cluster, best_silhouette);

    // Starting the timer for performance measurement
    tstart = omp_get_wtime();
    k_medoids(data, data_size, best_cluster);

    // Stopping the timer and print the result
    elapsed = omp_get_wtime() - tstart;
    printf("Elapsed time %f\n", elapsed);

    const char *header = "X,Y,Cluster\n";
    const char *filename = "output_mall_kmedoids.csv";
    
    create_marks_csv(data, data_size, filename, header);
    
    // Freeing memory for the array data
    for (int i = 0; i < data_size; ++i) {
        delete_x(&data[i]);
    }
    free(data);

    return 0;
}