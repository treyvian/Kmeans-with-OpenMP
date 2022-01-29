KMEDOIDS = ../kmedoids/
KMEANS = ../kmeans/
SILHOUETTE = ../silhouette-score/
RW = ../rw-csv/

OBJ_FILES = mall.o kmeans.o kmedoids.o silhouette.o rw.o

all: out

out: $(OBJ_FILES)
	gcc -o out $(OBJ_FILES) -lm -fopenmp

mall.o:
	gcc -c mall.c -fopenmp -o mall.o

kmeans.o: 
	gcc -c $(KMEANS)kmeans.c -fopenmp -o ./kmeans.o

kmedoids.o: 
	gcc -c $(KMEDOIDS)kmedoids.c -fopenmp -o ./kmedoids.o

silhouette.o: 
	gcc -c $(SILHOUETTE)silhouette.c -fopenmp -o ./silhouette.o

rw.o: 
	gcc -c $(RW)rw.c -o ./rw.o


clean:
	rm $(OBJ_FILES)
	rm out

