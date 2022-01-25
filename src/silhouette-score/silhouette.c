#include "silhouette.h"

double silhouette_score (point *data, int n, int k) {

    assert(data != NULL);

    double silhouette_score, Cohesion;

    double mean_coh, mean_sep, sep, distance;

    double *separation = (double *)calloc(k, sizeof(double));
    int *n_clust = (int *)calloc(k, sizeof(int));

    int cluster_number;

    for (int i=0; i < n; ++i) {

        Cohesion = 0;
        cluster_number = data[i].cluster;

        #pragma omp parallel for reduction(+:Cohesion, separation[:k]) default(none) firstprivate(cluster_number, i, n, k, distance) shared(data, n_clust) schedule(static, 64)
        for (int j = 0; j < n; ++j) {
            if (i != j) {            
                distance = euclidian_distance(&data[i], &data[j]);
                int cluster_j = data[j].cluster;
                
                if (cluster_number == cluster_j) {
                    Cohesion += distance;
                    #pragma omp atomic
                    n_clust[cluster_j]++;
                
                } else {
                    separation[cluster_j] += distance;
                    #pragma omp atomic
                    n_clust[cluster_j]++;
                }    
            }    
        }

        mean_coh = Cohesion / n_clust[cluster_number];
        mean_sep = __DBL_MAX__;

        for (int j = 0; j < k; ++j) {
            if (j != cluster_number) {
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

    free(separation);
    free(n_clust);  

    return silhouette_score / n;
}
