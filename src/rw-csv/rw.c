#include "rw.h"

void read_csv (int row, int col, const char *filename, double **data) {
	
    FILE *file;
	file = fopen (filename, "r") ;
    assert(file != NULL);

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

void create_marks_csv(double **points, int *clusters, int n, int dimensions, const char *name, const char *header){
    
    FILE *fp;

    chdir("output/");

    fp = fopen(name,"w+");
    assert(fp != NULL);

    fprintf(fp, header); 

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < dimensions; ++j) {
            fprintf(fp,"%.3f,", points[i][j]);
        }
        fprintf(fp,"%d\n", clusters[i]);
    }   
    
    fclose(fp);
}