#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

#include "silhouette.h"

double silhouette_score (point *data, int n, int k) {

    assert(data != NULL);
    
    int numThreads = omp_get_num_threads();
    printf("%d", numThreads);
    omp_set_num_threads(numThreads);

    omp_set_nested(1);


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

        #pragma omp parallel for reduction(+:Cohesion, Separation[:k], n_clust[:k]) schedule(static)
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
