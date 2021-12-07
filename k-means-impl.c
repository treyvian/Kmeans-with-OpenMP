#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef struct point {
    double *x;
    int dimensions;
    int cluster;
    double minDist;
} point;


point *point_in (double *x, int dimensions) {
    point *p = (point *)malloc(sizeof(point));
    if (p) {
        p->x = x;
        p->dimensions = dimensions;
        p->cluster = -1;
        p->minDist = __DBL_MAX__;
    }
    return p;
}

double distance (point *p1, point *p2) {
    double dist;
    
    for (int i=0; i< p1->dimensions; i++) {
        dist += (p2->x[i] - p1->x[i]) * (p2->x[i] - p1->x[i]);
    }
    
    return sqrt(dist);
}

void print_point (point *p) {
    printf("(");
    
    for (int i=0; i < p->dimensions; i++) {
        printf("%f ", p->x[i]);
    }

    printf(")\n");
}

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
        
        for (int i=0; i<k; i++) {
            int cluster_id = i;

            for (int j=0; j<n; j++) {
                double dist = distance(&centroids[i], &points[j]);
                
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
        printf("\n");
    }
}


void create_marks_csv(char *filename, point *points,int n){
 
    printf("Creating %s.csv file\n",filename);
    
    FILE *fp;
    
    //filename = strcat(filename,".csv");

    fp=fopen(filename,"w+");

    fprintf(fp,"X,Y,Cluster\n");  
    
    fclose(fp);
    
    printf("\n %sfile created\n",filename);
}
 

int main (int argc, char const *argv[]) {

    // Reading the csv file
    if (argc < 3) {
		printf("Please specify the CSV file as an input.\n");
		exit(0);
	}

	int row     = atoi(argv[1]);
	int col     = atoi(argv[2]);
	char fname[256];	strcpy(fname, argv[3]);

	int **dat;
	dat = (int **)malloc(row * sizeof(int *));
	for (int i = 0; i < row; ++i){
		dat[i] = (int *)malloc(col * sizeof(int));
	}

	read_csv(row, col, fname, dat);

    const size_t data_size = row;

    point *data = (point *)malloc(data_size * sizeof(point));

    double *p = (double *)malloc(2 * sizeof(double));

    for (int i = 0; i < data_size; ++i) { 
        p[0] = (double) dat[i][3];
        p[1] = (double) dat[i][4];
        data[i] = *point_in(p, 2);
    }
    
    // freeing memory for the array dat
    for (int i=0; i < data_size; i++) {
        free(dat[i]);
    }
    free(dat);
    
    // kMeansClustering(data, data_size, 100, 5);

    // char *name = "output.csv";

    // create_marks_csv(name,data,data_size);

    

    
    // Freeing memory for the array data
    free(data);

    return 0;
}