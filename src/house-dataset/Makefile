POINT = ../point/
KMEDOIDS = ../kmedoids/
KMEANS = ../kmeans/
SILHOUETTE = ../silhouette-score/
RW = ../rw-csv/

OBJ_FILES = house.o point.o kmeans.o kmedoids.o silhouette.o rw.o

all: out

out: $(OBJ_FILES)
	gcc -o out $(OBJ_FILES) -lm -fopenmp -O3

house.o:
	gcc -c house.c -fopenmp -O3 -o house.o 

point.o: 
	gcc -c $(POINT)point.c -O3 -o ./point.o 

kmeans.o: 
	gcc -c $(KMEANS)kmeans.c -fopenmp -O3 -o ./kmeans.o

kmedoids.o: 
	gcc -c $(KMEDOIDS)kmedoids.c -fopenmp -O3 -o ./kmedoids.o

silhouette.o: 
	gcc -c $(SILHOUETTE)silhouette.c -fopenmp -O3 -o ./silhouette.o

rw.o: 
	gcc -c $(RW)rw.c -O3 -o rw.o


clean:
	rm out
	rm $(OBJ_FILES)