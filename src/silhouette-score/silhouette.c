#include "silhouette.h"

int dimensions_point;

double euclidian_distance (const double *p1, const double *p2) {

    double distance = 0;

    for (int i = 0; i < dimensions_point; ++i) {
        distance += pow((p2[i] - p1[i]),2);
    }
    
    return sqrt(distance);
}


double silhouette_score (double **data, int *clusters, int n, int dimensions,int k) {

    // 5 arguments
    // data       - n x dimensions size array containing the data in input
    // clusters   - n-dimensional array containing the cluster number of the 
    //              corresponding point
    // n          - number of points in input
    // dimensions - dimension of the point in input
    // k          - number of cluster to divide the dataset into

    assert(data != NULL);
    assert(clusters != NULL);

    double silhouette_score = 0;
    dimensions_point = dimensions;

    double mean_coh, mean_sep, sep, distance, Cohesion;

    double separation[k];
    int n_clust[k];
    
    for(int i=0; i < k; ++i) {
        separation[i] = 0.0;
        n_clust[i] = 0; 
    }

    /*
    * For each point calculates the cohesion and the separation with respect to
    * all the others points
    */
    #pragma omp parallel for reduction(+:silhouette_score) private(mean_coh, mean_sep, sep, distance, Cohesion) firstprivate(n, k, separation, n_clust, dimensions) schedule(static)
    for (int i=0; i < n; ++i) {

        Cohesion = 0;

        // Find cohesion and separation of each point in the cluster w.r.t. the i-th point
        for (int j = 0; j < n; ++j) {
            if (i != j) {           
                distance = euclidian_distance(data[i], data[j]);
                
                if (clusters[i] == clusters[j]) {
                    Cohesion += distance;
                    n_clust[clusters[j]]++;
                
                } else {
                    separation[clusters[j]] += distance;
                    n_clust[clusters[j]]++;
                }    
            }    
        }

        mean_coh = Cohesion / n_clust[clusters[i]];
        mean_sep = __DBL_MAX__;

        for (int j = 0; j < k; ++j) {
            if (j != clusters[i]) {
                sep = separation[j] / n_clust[j];
                if (mean_sep > sep){
                    mean_sep = sep;
                }
            } 
            separation[j] = 0.0;
            n_clust[j] = 0;   
        }


        if (sep > mean_coh) {
            silhouette_score += (mean_sep - mean_coh) / mean_sep;
        } else silhouette_score += (mean_sep - mean_coh) / mean_coh;
    }

    return silhouette_score / n;
}
