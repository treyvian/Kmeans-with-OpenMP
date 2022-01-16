#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <error.h>
#include <string.h>

#include "rw.h"

void read_csv (int row, int col, const char *filename, double **data) {
	
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

void create_marks_csv(point *points, int n, const char *name, const char *header){
 
    printf("Creating output.csv file\n");
    
    FILE *fp;

    chdir("output/");

    fp = fopen(name,"w+");
    
    if (!fp) {    
      error(EXIT_FAILURE, errno, "Failed to open file for writing");
    }

    fprintf(fp, header); 

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < points->dimensions; ++j) {
            fprintf(fp,"%.3f,", points[i].x[j]);
        }
        fprintf(fp,"%d\n", points[i].cluster);
    }   
    
    fclose(fp);
    printf("\n output.csv file created\n");
}