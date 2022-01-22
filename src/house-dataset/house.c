#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <time.h> /* for time() */
#include <omp.h>

/*
* My methods
*/
#include "../point/point.h"
#include "../rw-csv/rw.h"

void k_means (point *points, 
                const int n, 
                const int epochs, 
                const int k) {
    
    assert(points != NULL);

    assert(epochs > 0);

    point *centroids = (point *)calloc(k, sizeof(point));
    assert(centroids != NULL);

    int bool, random, iter, cluster_num;
    int points_dimensions = points->dimensions;

    srand(time(0));
    for (int i = 0; i < k; i++) {
        bool = 1;
        random = rand() % n;
        for (int j = 0; j < i; j++) {
            if (equals(&points[random], &centroids[j])) {
                bool = 0;
                i--;
                break;
            }    
        }
        if (bool) {
            copy_point(&centroids[i], &points[random]);
        }
    }
    
    int *nPoints = (int *)malloc(k * sizeof(int));
    assert(nPoints != NULL);

    double **sum = (double **)malloc(k * sizeof(double *));
    assert(sum != NULL);

    #pragma omp parallel for 
    for (int i = 0; i < k; ++i) {
	    sum[i] = (double *)malloc(points_dimensions * sizeof(double));
        assert(sum[i] != NULL);
	}
        
    for (int t = 0; t<epochs; t++) {
        double distance;

        // Initialise with zeroes
        for (int j = 0; j < k; ++j) {
            nPoints[j] = 0;
            for (int i = 0; i< points_dimensions; i++) {
                sum[j][i] = 0.0;
            }    
        }

        // Updating the distance of each point with respect to the current centroids
        #pragma omp parallel for private(distance, cluster_num) schedule(static)
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < k; ++j) {
                distance = euclidian_distance(&centroids[j], &points[i]);

                if (distance < points[i].min_distance){
                    points[i].min_distance = distance;
                    points[i].cluster = j;
                    cluster_num = j; 
                }
            }

            // Iterate over points to append data to centroids
            #pragma omp critical 
            {
                nPoints[cluster_num]++;
            
                for (int j = 0; j < points_dimensions; ++j) {
                    sum[cluster_num][j] += points[i].x[j]; 
                }
            
                points[i].min_distance = __DBL_MAX__;
            }
        }
        
        bool = 1;

        // Compute new centroids
        double prov_sum;
        for (int i = 0; i < k; ++i) {
            int nP = nPoints[i];

            for (int j = 0; j < points_dimensions; j++) {
                prov_sum = sum[i][j]/nP;
                if (centroids[i].x[j] != prov_sum) {
                    bool = 0;
                    centroids[i].x[j] = prov_sum;
                }    
            }
        }

        if (bool) {
            // Freeing points, sum and centroids
            for (int i = 0; i < k; ++i) {
                free(sum[i]);
                delete_point(&centroids[i]);
            }
            free(sum);
            free(centroids); 
            free(nPoints); 
            
            return;
        }

        iter = t + 1;
    }
    
    // Freeing points, sum and centroids
    for (int i = 0; i < k; ++i) {
        free(sum[i]);
        delete_point(&centroids[i]);
    }
    free(sum);
    free(centroids); 
    free(nPoints);  
}

double silhouette_score (point *data, int n, int k) {

    assert(data != NULL);

    double silhouette_score = 0;

    #pragma omp parallel for reduction(+:silhouette_score) schedule(static)
    for (int i=0; i < n; ++i) {

        double Cohesion, mean_coh, Separation[k], mean_sep, sep, distance;

        int cluster_number, n_clust[k];

        Cohesion = 0;
        cluster_number = data[i].cluster;
        
        for (int t = 0; t < k; ++t) {
            Separation[t] = 0.0;
            n_clust[t] = 0;
        }

        // #pragma omp parallel for reduction(+:Cohesion, Separation[:k], n_clust[:k]) schedule(static)
        for (int j = 0; j < n; ++j) {
            if (i != j) {            
                distance = euclidian_distance(&data[i], &data[j]);
                
                if (cluster_number == data[j].cluster) {
                    Cohesion += distance;
                    n_clust[data[j].cluster]++;
                
                } else {
                    Separation[data[j].cluster] += distance;
                    n_clust[data[j].cluster]++;
                }    
            }    
        }

        mean_coh = Cohesion / n_clust[cluster_number];
        mean_sep = __DBL_MAX__;

        for (int j = 0; j < k; ++j) {
            if (j != cluster_number) {
                sep = Separation[j] / n_clust[j];
                if (mean_sep > sep){
                    mean_sep = sep;
                }
            }    
        }

        if (sep > mean_coh) {
            silhouette_score += (mean_sep - mean_coh) / mean_sep;
        } else silhouette_score += (mean_sep - mean_coh) / mean_coh;
    }
    return silhouette_score / n;
}


void k_medoids (point *points, 
                        const int n, 
                        const int k) {

    assert(points != NULL);

    point *medoids = (point *)calloc(k, sizeof(point));
    assert(medoids != NULL);

    point *best_medoids = (point *)calloc(k, sizeof(point));
    assert(best_medoids != NULL);

    int random, iter, boolean;
    int points_dimensions = points->dimensions;

    srand(time(0));
    for (int i = 0; i < k; i++) {
        boolean = 1;
        random = rand() % n;
        for (int j = 0; j < i; j++) {
            if (equals(&points[random], &medoids[j])) {
                boolean = 0;
                i--;
                break;
            }    
        }
        if (boolean) {
            copy_point(&medoids[i], &points[random]);
        }
    }

    double total_cost = __DBL_MAX__;

    double distance, new_total_cost;


    for (int i=0; i<k; ++i) {
        copy_point(&best_medoids[i], &medoids[i]);
    }

    #pragma omp parallel for private(distance) firstprivate(medoids) schedule(static)
    for (int i = 0; i<k; ++i){
        for (int j = 0; j<n; ++j){
            if (!equals(&points[j], &medoids[i])) {            
                new_total_cost = 0;
                distance = 0;
                copy_point(&medoids[i], &points[j]);

                for (int t = 0; t < n; ++t) {
                    for (int r = 0; r < k; ++r) {
                        distance = manhattan_distance(&medoids[r], &points[t]);

                        if (points[t].min_distance > distance) {
                            points[t].min_distance = distance; 
                        }
                    }

                    new_total_cost += points[t].min_distance; 

                    points[t].min_distance = __DBL_MAX__;
                }

                #pragma omp critical
                {
                    if (total_cost > new_total_cost) {
                        total_cost = new_total_cost;
                        copy_point(&best_medoids[i], &points[j]);
                    } else {
                        copy_point(&medoids[i], &best_medoids[i]);
                    }
                }    
            }                      
        }

        #pragma omp critical
        {        
        copy_point(&medoids[i], &best_medoids[i]);
        }
    }

    #pragma omp parallel for private(distance) schedule(static)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < k; ++j) {
            distance = manhattan_distance(&best_medoids[j], &points[i]);

            if (distance < points[i].min_distance){
                points[i].cluster = j;
                points[i].min_distance = distance;
            }
        }
           
        points[i].min_distance = __DBL_MAX__;
    }

    for (int i = 0; i < k; ++i) {
        delete_point(&best_medoids[i]);
        delete_point(&medoids[i]);
    }
    free(best_medoids);
    free(medoids);
}

int main (int argc, char const *argv[]) {

    // Check for the minimum number of input parameters
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
    int dimensions = 3;

    // Initialize the data array with one point for each row
    for (int i = 1; i < row; ++i) {
        supp_vector = (double *)malloc(dimensions * sizeof(double)); 
        supp_vector[0] = (double) dat[i][0];
        supp_vector[1] = (double) dat[i][6];
        supp_vector[2] = (double) dat[i][7];
        point_init(&data[i-1], supp_vector, dimensions);
    }

    // Freeing support array dat
    for (int i=0; i < row; i++) {
        free(dat[i]);
    }
    free(dat);

    const size_t data_size = row-1;

    /*
    * K-means
    */
    int max_iterations = 500;
    double tstart, elapsed;

    // Starting the timer for performance measurement
    tstart = omp_get_wtime();
    
    // Clustering the dataset
    k_means(data, data_size, max_iterations, 6);
    
    // Stopping the timer and print the result
    elapsed = omp_get_wtime() - tstart;
    printf("Elapsed time for k-means method: %f\n", elapsed);

    // Starting the timer for performance measurement 
    tstart = omp_get_wtime();
    
    silhouette_score(data, data_size, 6);

    // Stopping the timer and print the result
    elapsed = omp_get_wtime() - tstart;
    printf("Elapsed time silhouette %f\n", elapsed);

    // Header of the csv file in output
    char *header = "MedInc,Latitude,Longitude,Cluster\n";
    
    // Name of the csv file in output
    char *filename = "output_house_kmeans.csv";

    // Creating the file in output
    create_marks_csv(data, data_size, filename, header);    

    /*
    * K-medoids
    */
    // Starting the timer for performance measurement
    tstart = omp_get_wtime();

    // Clustering the dataset
    k_medoids(data, data_size, 6);

    // Stopping the timer and print the result
    elapsed = omp_get_wtime() - tstart;
    printf("Elapsed time kmedoids %f\n", elapsed);

    // Starting the timer for performance measurement 
    tstart = omp_get_wtime();
    silhouette_score(data, data_size, 6);

    // Stopping the timer and print the result
    elapsed = omp_get_wtime() - tstart;
    printf("Elapsed time silhouette %f\n", elapsed);

    header = "MedInc,Latitude,Longitude,Cluster\n";
    filename = "output_housing_kmedoids.csv";
    
    // Creating the file in output
    create_marks_csv(data, data_size, filename, header);

    
    // Freeing memory for the array data
    for (int i = 0; i < data_size; ++i) {
        delete_point(&data[i]);
    }
    free(data);

    return 0;
}