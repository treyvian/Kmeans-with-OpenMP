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

  if (end > points_size)
  {
    end = points_size;
    length_per_thread = start - end;
  }

  double min_distance;
  int min_index;

  for (int i = start; i < end; i++) {

    double *point = points[i];
    min_distance = __DBL_MAX__;
    min_index = 0;

    for (int j = 0; j < clusters_size; j++) {
      double distance = euclidean_dist(point, centroids[j]);

      if (distance < min_distance) {
        min_distance = distance;
        min_index = j;
      }
    }
    
    clusters[i] = min_index;

    #pragma omp atomic
    num_points_cluster[min_index]++;
    
    #pragma omp critical
    {  
      for (int j = 0; j < point_dimension; j++) {
        sum_points[min_index][j] += point[j];
      }
    }
  }
}

int update_clusters(double **centroids, 
                    int *num_point_clusters, 
                    double **sum_points) {

  int conv = 0;

  for(int i = 0; i < clusters_size; i++){
    for (int j = 0; j < point_dimension; j++){
      double new_value = sum_points[i][j]/(double)num_point_clusters[i];
      if(centroids[i][j] != new_value) {
        centroids[i][j] = new_value;
        conv = 1; 
      } 
      sum_points[i][j] = 0;
    }
    num_point_clusters[i] = 0;
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

  num_threads = omp_get_num_threads();

  point_dimension = dimensions;
  points_size = n;
  clusters_size = k;

  double **centroids = (double **)malloc(clusters_size * sizeof(double*));
  assert(centroids);
  int *num_point_clusters = (int *)calloc(clusters_size, sizeof(int));
  double **sum_points = (double **)malloc(clusters_size * sizeof(double *));

  for (int i = 0; i < k; i++) {
    centroids[i] = (double *)malloc(point_dimension * sizeof(double));
    assert(centroids[i]);
    sum_points[i] = (double *)calloc(point_dimension, sizeof(double));
    assert(sum_points[i]);
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
    
      #pragma omp barrier

      #pragma omp task
      iterations ++;

      #pragma omp master
      conv = update_clusters(centroids, num_point_clusters, sum_points);
    }
  }

  for (int i = 0; i < k; i++){
    free(centroids[i]);
    free(sum_points[i]);
  }
  free(centroids);
  free(num_point_clusters);
  free(sum_points);
}