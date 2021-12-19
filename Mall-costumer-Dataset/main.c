#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "../k-means/kmeans.h"

void read_csv (int row, int col, char *filename, int **data) {
	
    FILE *file;
	file = fopen (filename, "r") ;

	int i = 0;
    char line[4098];
	while (fgets(line, 4098, file) && (i < row)) {
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

int main (int argc, char const *argv[]) {
    
    // Reading the csv file
    if (argc < 3) {
		printf("Please specify the CSV file as an input.\n");
		exit(0);
	}

	int row = atoi(argv[1]);
	int col = atoi(argv[2]);
	char fname[256];	
    strcpy(fname, argv[3]);

	int **dat;
	dat = (int **)malloc(row * sizeof(int *));
	for (int i = 0; i < row; ++i){
		dat[i] = (int *)malloc(col * sizeof(int));
	}

	read_csv(row, col, fname, dat);

    int n_clusters = atoi(argv[4]);    

    point *data = (point *)malloc((row - 1) * sizeof(point));

    double *x;

    for (int i = 1; i < row; ++i) {
        x = (double *)malloc(2 * sizeof(double)); 
        x[0] = (double) dat[i][3];
        x[1] = (double) dat[i][4];
        point_init(&data[i-1], x, 2);
    }

    const size_t data_size = row-1;

    // freeing memory for the array dat
    for (int i=0; i < data_size; i++) {
        free(dat[i]);
    }
    free(dat);
    
    
    if (n_clusters < 1) {
        print("Number of clusters inserted not valid\n");
        exit(1);    
    }

    

    // KMeans implementations
    for (size_t i = 0; i < n_clusters; i++) {
        kMeansClustering(data, data_size, 100, i);
    }

    char *name = "output.csv";

    create_marks_csv(name,data,data_size);
    
    // Freeing memory for the array data

    for (int i = 0; i < data_size; ++i) {
        delete_x(&data[i]);
    }
    free(data);

    return 0;
}