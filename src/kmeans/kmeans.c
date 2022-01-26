#include "kmeans.h"

void k_means (point *points, 
                const int n, 
                const int epochs, 
                const int k) {
    
    assert(points != NULL);

    assert(epochs > 0);

    point *centroids = (point *)calloc(k, sizeof(point));
    assert(centroids != NULL);

    int boolean, random;
    int points_dimensions = points->dimensions;

    srand(time(0));
    for (int i = 0; i < k; i++) {
        boolean = 1;
        random = rand() % n;
        
        for (int j = 0; j < i; j++) {
            if (equals(&points[random], &centroids[j])) {
                boolean = 0;
                i--;
                break;
            }    
        }
        if (boolean) {
            copy_point(&centroids[i], &points[random]);
        }
    }
    
    int nPoints[k];

    double sum[k][points_dimensions];
    
    for (int i = 0; i < k; ++i) {
        nPoints[i] = 0;
        for (int j = 0; j < points_dimensions; j++) {
            sum[i][j] = 0.0;
        }
	}
    
    boolean = 1;
    #pragma omp parallel firstprivate(boolean)
    for (int t = 0; boolean && t < epochs; t++) {
           
        // Updating the distance of each point with respect to the current centroids
        #pragma omp for firstprivate(n, k, points_dimensions, centroids) schedule(static, 64)
        for (int i = 0; i < n; ++i) {
            int cluster_num;
            for (int j = 0; j < k; ++j) {
                double distance = euclidian_distance(&centroids[j], &points[i]);

                if (distance < points[i].min_distance){
                    points[i].min_distance = distance;
                    points[i].cluster = j;
                    cluster_num = j; 
                }
            }
            
            nPoints[cluster_num]++;

            // Iterate over points to append data to centroids
            for (int j = 0; j < points_dimensions; ++j) {
                sum[cluster_num][j] += points[i].x[j];
            }

            points[i].min_distance = __DBL_MAX__;
        }

        boolean = 1;
        // Compute new centroids
        #pragma omp for firstprivate(k, points_dimensions)
        for (int i = 0; i < k; ++i) {
            int nP = nPoints[i];
            nPoints[i] = 0;

            for (int j = 0; j < points_dimensions; j++) {
                double prov_sum = sum[i][j]/nP;
                sum[i][j] = 0.0;
                if (centroids[i].x[j] != prov_sum) {
                    #pragma omp critical
                    {    
                        boolean = 1;
                        centroids[i].x[j] = prov_sum;
                    }
                }    
            }
        }
    }

    // Freeing points, sum and centroids
    for (int i = 0; i < k; ++i) {
        delete_point(&centroids[i]);
    }
    free(centroids);
}
