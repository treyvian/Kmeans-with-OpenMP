#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "k-means.h"


void read_csv (int row, int col, char *filename, int **data) {
	
    FILE *file;
	file = fopen (filename, "r") ;

	int i = 0;
    char line[4098];
	while (fgets(line, 4098, file) && (i < row))
    {
    	// double row[ssParams->nreal + 1];
        char* tmp = strdup(line);

	    int j = 0;
	    const char* tok;
	    for (tok = strtok(line, ","); tok && *tok; j++, tok = strtok(NULL, ","))
	    {
	        data[i][j] = atof(tok);
	    }

        free(tmp);
        i++;
    }
}


void kMeansClustering (point *points, 
                        const int n, 
                        const int epochs, 
                        const int k) {

    if (points == NULL){
        exit;
    }
    
    point *centroids = (point *)malloc(k * sizeof(point));
    srand(time(0));
    for (int i = 0; i < k; ++i) {
        centroids[i] = points[rand() % n];
    }
    
    int *nPoints = (int *)malloc(k * sizeof(int));
    double **sum = (double **)malloc(k * sizeof(double *));
    for (int i = 0; i < k; ++i) {
	    sum[i] = (double *)malloc(points->dimensions * sizeof(double));
	}
    
    for (int t = 0; t<epochs; t++) {
        double dist;
        int cluster_id;
        for (int i = 0; i < k; ++i) {
            cluster_id = i;

            for (int j = 0; j < n; j++) {
                dist = distance(centroids[i], points[j]);
        
                if (dist < points[j].minDist) {
                    points[j].minDist = dist;
                    points[j].cluster = cluster_id;
                }
            }
        }
    
        // Initialise with zeroes
        for (int j = 0; j < k; ++j) {
            nPoints[j] = 0;
            for (int i = 0; i< points->dimensions; i++) {
                sum[j][i] = 0.0;
            }    
        }
        
        // Iterate over points to append data to centroids
        for (int i=0; i<n; i++) {
            int cluster_id = points[i].cluster;
            nPoints[cluster_id] += 1;
            
            for (int j = 0; j < points->dimensions; ++j) {
                sum[cluster_id][j] += points[i].x[j]; 
            }

            points[i].minDist = __DBL_MAX__;
        }

        // Compute new centroids
        for (int i=0; i<k; ++i) {
            int nP = nPoints[i];

            for (int j = 0; j<centroids->dimensions; j++) {
                centroids[i].x[j] = sum[i][j]/nP;
                //printf("%f ", centroids[i].x[j]);
            }
        }
    }
}


void create_marks_csv(char *filename, point *points,int n){
 
    printf("Creating %s.csv file\n",filename);
    
    FILE *fp;
    
    //filename = strcat(filename,".csv");

    fp=fopen(filename,"w+");

    fprintf(fp,"X,Y,Cluster\n"); 

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < points->dimensions; ++j) {
            fprintf(fp,"%f,", points[i].x[j]);
        }
        fprintf(fp,"%d\n", points[i].cluster);
    }   
    
    fclose(fp);
    
    printf("\n %sfile created\n",filename);
}
 