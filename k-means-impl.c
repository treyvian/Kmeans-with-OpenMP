#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>


struct point {
    double x, y;
    int cluster;
    double minDist;
};

typedef struct point point;

point *point_in(double x, double y){
    point *p = (point *)malloc(sizeof(point));
    p->x = x;
    p->y = y;
    p->cluster = -1;
    p->minDist = __DBL_MAX__;
    return p;
}

double distance(point p1, point p2){
    return (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
}

void print(point *p){
    printf("(%f,%f)\n", p->x, p->y);
}

void read_csv(int row, int col, char *filename, int **data){
	FILE *file;
	file = fopen(filename, "r");

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


void kMeansClustering(point *points, const int n, const int epochs, const int k){
    point *centroids = (point *)malloc(k * sizeof(point));
    srand(time(0));
    for (int i = 0; i < k; ++i) {
        centroids[i] = points[(rand() % n)];
    }

    for (int t = 0; t<epochs; t++) {
        
        for (int i=0; i<k; i++) {
            int cluster_id = i;

            for (int j=0; j<n; j++) {
                double dist = distance(centroids[i], points[j]);
                if (dist < points[j].minDist){
                    points[j].minDist = dist;
                    points[j].cluster = cluster_id;
                }
            }
        }

        //for (int i=0; i<n; i++){
        //    printf("%f - %d\n",points[i].minDist, points[i].cluster);
        //}

        int *nPoints = (int *)malloc(k * sizeof(int));
        double *sumX = (double *)malloc(k * sizeof(double)); 
        double *sumY = (double *)malloc(k * sizeof(double));

        // Initialise with zeroes
        for (int j = 0; j < k; ++j) {
            nPoints[j] = 0;
            sumX[j] = 0.0;
            sumY[j] = 0.0;
        }

        // Iterate over points to append data to centroids
        for (int i=0; i<n; i++) {
            int cluster_id = points[i].cluster;
            nPoints[cluster_id] += 1;
            sumX[cluster_id] += points[i].x; 
            sumY[cluster_id] += points[i].y;
        
            points[i].minDist = __DBL_MAX__;
        }

        // Compute new centroids
        for (int i=0; i<k; ++i) {
            int cluster_id = i;
            centroids[i].x = sumX[cluster_id]/nPoints[cluster_id];
            centroids[i].y = sumY[cluster_id]/nPoints[cluster_id];
        }
    }
}


void create_marks_csv(char *filename, point *points,int n){
 
    printf("Creating %s.csv file\n",filename);
    
    FILE *fp;
    
    //filename = strcat(filename,".csv");

    fp=fopen(filename,"w+");

    fprintf(fp,"X,Y,Cluster\n");
    
    for (int i=0;i<n;i++) {
    
        fprintf(fp,"%f,%f,%d\n", points[i].x, points[i].y, points[i].cluster);
    }    
    
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

    point *data;
    data = (point *)malloc((row) * sizeof(point));

    for (int i = 0; i < row; i++) {
        data[i] = *point_in(dat[i][3], dat[i][4]);
    }

    const size_t data_size = row;

    // freeing memory for the array dat
    for (int i=0; i < row; i++) {
        free(dat[i]);
    }
    free(dat);


    kMeansClustering(data, data_size, 100, 5);

    printf("prova");

    char *name = "output.csv";

    create_marks_csv(name,data,data_size);

    

    
    // Freeing memory for the array data
    free(data);

    return 0;
}