#include "kmeans.h"

int point_dimension; /* Dimensions of a point */
int num_threads; /* Max number of threads available for this run */
unsigned int points_size; /* Number of points in input */
unsigned int clusters_size; /* Number of cluster to find */


/*
* Help method to calculate the euclidean distance
*/
double euclidean_dist (double *p1, double *p2) {
    
    double distance = 0;

    for (int i = 0; i < point_dimension; ++i) {
      distance += (p2[i] - p1[i]) * (p2[i] - p1[i]);
    }
    
    return sqrt(distance);
}


/*
* Assign the cluster value to each point based on the centorids distance.
*/
void compute_distance(int tID,
                      double **points, 
                      double **centroids, 
                      int *clusters, 
                      int *num_points_cluster,
                      double **sum_points){
  
  // 6 arguments
  // tID                - number of the active thread
  // points             - Array containing the points in input
  // centroids          - Array containing the current values of the centroids
  // clusters           - contains the value of the cluster for each point
  // num_points_cluster - Contains the number of point for each cluster
  // sum_points         - sum of the coordinates for each point in each cluster

  // Calculate for the indexes for each thread
  int length_per_thread = points_size / num_threads;
  int start = tID * length_per_thread;
  int end = start + length_per_thread;
  int diff = points_size - (num_threads * length_per_thread);

  if (end > points_size) {
    end = points_size;
    length_per_thread = start - end;
  }

  if (tID == (num_threads - 1)) {
    end += diff;
  }


  double min_distance; /* Distance to the nearest cluster for each point*/
  double distance;
  int min_index; /* Index of the nearest cluster for each point*/

  // For each point in the range of the thread (start, end)
  double ts, te = 0;
  for (int i = start; i < end; i++) {
    min_distance = __DBL_MAX__;
    min_index = 0;

    
    // Find the nearest cluster
    for (int j = 0; j < clusters_size; j++) {
      ts = omp_get_wtime();
      distance = euclidean_dist(points[i], centroids[j]);
      te += omp_get_wtime() -ts;
      if (distance < min_distance) {
        min_distance = distance;
        min_index = j;
      }
    }
    

    clusters[i] = min_index;

    // Each cluster gets its own element of the array (num_points_cluster, sum_points) to avoid mutual exclusion
    num_points_cluster[(min_index * num_threads) + tID]++;

    for (int j = 0; j < point_dimension; j++) {
      sum_points[(min_index * num_threads) + tID][j] += points[i][j];
    }
  }
  //printf("(%f) ", te);
}

/*
* Updates the values of the centroids and reset array num_points_cluster and
* sum_points
*/
int update_clusters(double **centroids,
                    int *num_point_clusters, 
                    double **sum_points) {
  
  // 3 arguments
  // centroids          - Array containing the current values of the centroids
  // num_points_cluster - Contains the number of point for each cluster
  // sum_points         - sum of the coordinates for each point in each cluster

  int conv = 0; /* Boolean value to return for early stopping*/
  double new_value;
  
  // Two arrays to join the one in input 
  double sum[clusters_size][point_dimension];
  memset(sum, 0, sizeof(sum));
  int num_points[clusters_size];
  memset(num_points, 0, sizeof(num_points) );

  /* 
  * Rejoin the values that were splitted for each cluster in one single value
  * to avoid messing to much with indexes
  */
  for (int i = 0; i < clusters_size; i++) {
    for (int j = 0; j < num_threads; j++) {
      
      num_points[i] += num_point_clusters[(i * num_threads) + j];
      num_point_clusters[(i * num_threads) + j] = 0;

      for (int k = 0; k < point_dimension; k++) {  

        sum[i][k] += sum_points[(i * num_threads) + j][k];
        sum_points[(i * num_threads) + j][k] = 0; 
      }
    }
  }

  // Update centorids values
  for (int i = 0; i < clusters_size; i++) {
    for (int k = 0; k < point_dimension; k++) {
      new_value = sum[i][k]/(double)num_points[i];

      if(centroids[i][k] != new_value) {
          centroids[i][k] = new_value;
          conv = 1; 
      }
    }
  }

  return conv; /* Return the boolean values */
}

void k_means (double **points,
                int *clusters,
                const int n,
                const int dimensions, 
                const int epochs,
                const int k) {

  // 6 arguments
  // points     - n x dimensions size array contains the data in input
  // clusters   - n-dimensional array that at the end of the algorithm will
  //              containts the number of the cluster associated to the point
  // n          - number of points in input
  // dimensions - dimension of the point in input
  // epochs     - maximum number of iteration of the algorithm
  // k          - number of cluster to divide the dataset into

  // Asserting the prerequisites
  assert(points != NULL);
  assert(clusters != NULL);
  assert(epochs > 0);

  // Initializing the static variables
  point_dimension = dimensions; 
  points_size = n;
  clusters_size = k;
  num_threads = omp_get_max_threads();

  // Array containing the centroids
  double **centroids = (double **)malloc(clusters_size * sizeof(double*));
  assert(centroids);

  // Array containing the number of points assigned to each cluster
  int *num_point_clusters;
  num_point_clusters = (int *)calloc(clusters_size * num_threads, sizeof(int));
  assert(num_point_clusters);

  // Array useful for recalculating the new centroids
  double **sum_points;
  sum_points = (double **)malloc(clusters_size * num_threads * sizeof(double *));
  assert(sum_points);

  // Allocations and initializations of the arrays
  for (int i = 0; i < clusters_size; i++) {
    for(int j = 0; j < num_threads; j++) {
      sum_points[(i * num_threads)+j] = (double *)calloc(point_dimension, sizeof(double));
      assert(sum_points[(i * num_threads)+j]);
    }
    centroids[i] = (double *)malloc(point_dimension * sizeof(double));
    assert(centroids[i]);
    for (int j = 0; j < point_dimension; j++){ 
      centroids[i][j] = points[i][j];
    }
  }

  int conv = 1; /* Boolean values to stop the while loop in case of not improving centroids */
  int iterations = 0; /* Keeps count of the number of iterations of the while loop */

  /* Starting parallel section */
  #pragma omp parallel
  {
    while(conv && iterations < epochs){

      int tID = omp_get_thread_num(); /* Saving the number of the active thread*/

      // Assign the cluster value to each point based on the distance
      compute_distance(tID, points, centroids, clusters, num_point_clusters, sum_points);

      #pragma omp barrier

      #pragma omp single
      {
        #pragma omp task
        iterations ++; /* Update iteration value */

        // Update the centroids values and return the boolean for early stopping
        #pragma omp task
        conv = update_clusters(centroids, num_point_clusters, sum_points); 
      }
    }
  } 

  // Freeing the previously istanceted arrays
  for (int i = 0; i < clusters_size; i++){
    free(centroids[i]);
    for (int j = 0; j < num_threads; j++) {
      free(sum_points[(i * num_threads) + j]);
    }
  }
  free(centroids);
  free(num_point_clusters);
  free(sum_points);
}