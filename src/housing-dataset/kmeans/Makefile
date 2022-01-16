POINT = ../../point/
KMEANS = ../../k-means/
SILHOUETTE = ../../silhouette-score/
RW = ../../rw-csv/

all: out

out: housing_kmeans.o $(POINT)/point.o $(KMEANS)/kmeans.o  $(SILHOUETTE)/silhouette.o $(RW)/rw.o
	gcc -fopenmp -o out housing_kmeans.o $(POINT)/point.o $(KMEANS)/kmeans.o  $(SILHOUETTE)/silhouette.o $(RW)/rw.o -lm

housing_kmeans.o: housing_kmeans.c
	gcc -c housing_kmeans.c

point.o: 
	make -C $(POINT)

kmeans.o: 
	make -C $(KMEANS)

silhouette.o: 
	make -C $(SILHOUETTE)

rw.o: 
	make -C $(RW)


clean:
	rm out
	rm housing_kmeans.o
