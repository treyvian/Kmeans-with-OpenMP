#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include <time.h> /* for time() */
#include <omp.h>

#include "../../k-means/kmeans.h"
#include "../../silhouette-score/silhouette.h"
#include "../../rw-csv/rw.h"

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
    double tstart, elapsed;

    tstart = omp_get_wtime();
    k_means(data, data_size, max_iterations, 6);
    elapsed = omp_get_wtime() - tstart;

    printf("Elapsed time for k-means method: %f\n", elapsed);

    tstart = omp_get_wtime();
    double sil_score = silhouette_score(data, data_size, 6);
    elapsed = omp_get_wtime() - tstart;
    
    printf("Elapsed time for silhouette method: %f\n", elapsed);
    
    printf("With 6 clusters the silhouette score is: %f \n", sil_score);

    const char *header = "MedInc,Latitude,Longitude,Cluster\n";
    const char *filename = "output_house_kmeans.csv";

    create_marks_csv(data, data_size, filename, header);
    
    // Freeing memory for the array data
    for (int i = 0; i < data_size; ++i) {
        delete_x(&data[i]);
    }
    free(data);

    return 0;
}