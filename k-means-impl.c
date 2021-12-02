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


void kMeansClustering(point *points, const int n, int epochs, int k){
    point *centroids = (point *)malloc(k * sizeof(point));
    srand(time(0));
    for (int i = 0; i < k; ++i) {
        centroids[i] = points[(rand() % n)];
    }

    for (int i=0; i<k; i++){
        print(&centroids[i]);
    }

    for (int i=0; i<k; i++){
        int cluster_id = i+1;

        for (int j=0; j<n; j++){
            double dist = distance(centroids[i], points[j]);
            if (dist < points[i].minDist){
                points[i].minDist = dist;
                points[i].cluster = cluster_id;
            }
        }
    }

    
}

int main(int argc, char const *argv[]){

    // Reading the csv file
    if (argc < 3){
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

    for(int i = 0; i < row; i++){
        data[i] = *point_in(dat[i][3], dat[i][4]);
    }

    const size_t data_size = row;

    // freeing memory for the array dat
    for (int i=0; i < row; i++) {
        free(dat[i]);
    }
    free(dat);


    kMeansClustering(data, data_size, 5, 5);

    // Freeing memory for the array data
    free(data);

    return 0;
}