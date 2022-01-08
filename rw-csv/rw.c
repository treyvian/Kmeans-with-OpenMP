#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rw.h"

void read_csv (int row, int col, const char *filename, int **data) {
	
    FILE *file;
	file = fopen (filename, "r") ;
    if(file == NULL) {
      perror("Error opening file");
      exit(1);
    }

	int i = 0;
    char line[4098];
	while (fgets(line, 4098, file) && (i < row)) {
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

    fclose(file);
}

void create_marks_csv(point *points, int n, const char *header){
 
    printf("Creating output.csv file\n");
    
    FILE *fp;

    fp = fopen("output.csv","w+");
    if (fp == NULL) {
      perror("Error opening file");
      exit(1);
    }

    fprintf(fp, header); 

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < points->dimensions; ++j) {
            fprintf(fp,"%f,", points[i].x[j]);
        }
        fprintf(fp,"%d\n", points[i].cluster);
    }   
    
    fclose(fp);
    printf("\n output.csv file created\n");
}