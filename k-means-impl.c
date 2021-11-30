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

void point_in(point *p, double x, double y){
    p->x = x;
    p->y = y;
    p->cluster = -1;
    p->minDist = __DBL_MAX__;
}

double distance(point p1, point p2){
    return (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
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


int main(int argc, char const *argv[]){
    point p1, p2; 
    point_in(&p1, 0.0, 0.0);
    point_in(&p2, 3.0, 4.0);

    printf("%f\n", distance(p1,p2));

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

    int **data;
    data = (int **)malloc((row-1) * sizeof(int *));
	for (int i = 0; i < row; ++i){
		data[i] = (int *)malloc(2 * sizeof(int));
	}

    for(int i = 0; i < row; i++){
        for(int j=0; j < col; j++){
            if(j > 2)
                data[i][j-3] = dat[i][j];
        }
    }


    for(int i = 1; i < row-1; i++){
        for(int j=0; j < 2; j++){
            printf("%d\t", data[i][j]);
        }
        printf("\n"); 
    }

    return 0;
}