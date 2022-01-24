POINT = ../point/
KMEDOIDS = ../kmedoids/
KMEANS = ../kmeans/
SILHOUETTE = ../silhouette-score/
RW = ../rw-csv/

OBJ_FILES = house.o point.o kmeans.o kmedoids.o silhouette.o rw.o

all: out

out: $(OBJ_FILES)
	gcc -o out $(OBJ_FILES) -lm -fopenmp

house.o:
	gcc -c house.c -fopenmp -o house.o

point.o: 
	gcc -c $(POINT)point.c -fopenmp -o ./point.o

kmeans.o: 
	gcc -c $(KMEANS)kmeans.c -fopenmp -o ./kmeans.o

kmedoids.o: 
	gcc -c $(KMEDOIDS)kmedoids.c -fopenmp -o ./kmedoids.o

silhouette.o: 
	gcc -c $(SILHOUETTE)silhouette.c -fopenmp -o ./silhouette.o

rw.o: 
	gcc -c $(RW)rw.c -o rw.o


clean:
	rm out
	rm *.o