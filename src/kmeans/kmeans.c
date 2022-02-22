#include "kmeans.h"

int point_dimension;
int num_threads;
unsigned int points_size;
unsigned int clusters_size;

double euclidean_dist (const double *p1, const double *p2) {
    
    double distance = 0;

    for (int i = 0; i < point_dimension; ++i) {
        distance += pow((p2[i] - p1[i]),2);
    }
    
    return sqrt(distance);
}

void compute_distance(int tID,
                      double **points, 
                      double **centroids, 
                      int *clusters, 
                      int *num_points_cluster,
                      double **sum_points){

  int length_per_thread = points_size / num_threads;
  int start = tID * length_per_thread;
  int end = start + length_per_thread;

  if (end > points_size) {
    end = points_size;
    length_per_thread = start - end;
  }

  double min_distance;
  double distance;
  int min_index;

  double tstart, elapsed;
  for (int i = start; i < end; i++) {
    double *point = points[i];
    min_distance = __DBL_MAX__;
    min_index = 0;

    
    for (int j = 0; j < clusters_size; j++) {
      distance = euclidean_dist(point, centroids[j]);

      if (distance < min_distance) {
        min_distance = distance;
        min_index = j;
      }
    }

    clusters[i] = min_index;

    num_points_cluster[(min_index * num_threads) + tID]++;

    for (int j = 0; j < point_dimension; j++) {
      sum_points[(min_index * num_threads) + tID][j] += point[j];
    }
  }
}

int update_clusters(double **centroids,
                    int *num_point_clusters, 
                    double **sum_points) {

  int conv = 0;
  double new_value;
  double sum[clusters_size][point_dimension];
  memset(sum, 0, sizeof(sum));
  int num_points[clusters_size];
  memset(num_points, 0, sizeof(num_points) );


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

  for (int i = 0; i < clusters_size; i++) {
    for (int k = 0; k < point_dimension; k++) {
      new_value = sum[i][k]/(double)num_points[i];

      if(centroids[i][k] != new_value) {
          centroids[i][k] = new_value;
          conv = 1; 
      }
    }
  }

  return conv;
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

  assert(points != NULL);
  assert(clusters != NULL);
  assert(epochs > 0);

  point_dimension = dimensions;
  points_size = n;
  clusters_size = k;
  num_threads = omp_get_max_threads();

  double **centroids = (double **)malloc(clusters_size * sizeof(double*));
  assert(centroids);

  int *num_point_clusters;
  num_point_clusters = (int *)calloc(clusters_size * num_threads, sizeof(int));
  assert(num_point_clusters);

  double **sum_points;
  sum_points = (double **)malloc(clusters_size * num_threads * sizeof(double *));
  assert(sum_points);

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

  int conv = 1;
  int iterations = 0;

  while(conv && iterations < epochs){
    
    #pragma omp parallel
    {
      int tID = omp_get_thread_num();
      compute_distance(tID, points, centroids, clusters, num_point_clusters, sum_points);
    }

    iterations ++;

    conv = update_clusters(centroids, num_point_clusters, sum_points);
  }


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