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
    
    int *nPoints = (int *)calloc(k, sizeof(int));
    assert(nPoints != NULL);

    double **sum = (double **)calloc(k, sizeof(double *));
    assert(sum != NULL);

    for (int i = 0; i < k; ++i) {
	    sum[i] = (double *)calloc(points_dimensions, sizeof(double));
        assert(sum[i] != NULL);
	}
    
    int iter = 0;
    int cluster_num;
    while (boolean && iter < epochs) {
        double distance;

        // Updating the distance of each point with respect to the current centroids
        #pragma omp parallel for default(none) shared(points, sum, nPoints) private(distance, cluster_num) firstprivate(n, k, points_dimensions, centroids) schedule(static, 64)
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < k; ++j) {
                distance = euclidian_distance(&centroids[j], &points[i]);

                if (distance < points[i].min_distance){
                    points[i].min_distance = distance;
                    points[i].cluster = j;
                    cluster_num = j; 
                }
            }
            
            #pragma omp critical
            {
                nPoints[cluster_num]++;

                // Iterate over points to append data to centroids
                for (int j = 0; j < points_dimensions; ++j) {
                    sum[cluster_num][j] += points[i].x[j];
                }

                points[i].min_distance = __DBL_MAX__;
            }
        }

        // Compute new centroids
        double prov_sum;
        for (int i = 0; i < k; ++i) {
            int nP = nPoints[i];
            nPoints[i] = 0;

            for (int j = 0; j < points_dimensions; j++) {
                prov_sum = sum[i][j]/nP;
                sum[i][j] = 0.0;
                if (centroids[i].x[j] != prov_sum) {
                    boolean = 0;
                    centroids[i].x[j] = prov_sum;
                }    
            }
        }

        iter++;
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
