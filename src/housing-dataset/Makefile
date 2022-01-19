POINT = ../point/
KMEANS = ../k-means/
KMEDOIDS = ../k-medoids/
SILHOUETTE = ../silhouette-score/
RW = ../rw-csv/

all: out

out: housing.o $(POINT)/point.o $(KMEANS)/kmeans.o $(KMEDOIDS)/kmedoids.o $(SILHOUETTE)/silhouette.o $(RW)/rw.o
	gcc -fopenmp -o out housing.o $(POINT)/point.o $(KMEANS)/kmeans.o $(KMEDOIDS)/kmedoids.o $(SILHOUETTE)/silhouette.o $(RW)/rw.o -lm

housing.o: housing.c
	gcc -c housing.c

point.o: 
	make -C $(POINT)

kmeans.o: 
	make -C $(KMEANS)

kmedoids.o:
	make -C $(KMEDOIDS)

silhouette.o: 
	make -C $(SILHOUETTE)

rw.o: 
	make -C $(RW)


clean:
	rm out
	rm housing.o
