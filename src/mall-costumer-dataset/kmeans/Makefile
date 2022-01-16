POINT = ../../point/
KMEANS = ../../k-means/
SILHOUETTE = ../../silhouette-score/
RW = ../../rw-csv/

all: out

out: mall_kmeans.o $(POINT)/point.o $(KMEANS)/kmeans.o  $(SILHOUETTE)/silhouette.o $(RW)/rw.o
	gcc -fopenmp -o out mall_kmeans.o $(POINT)/point.o $(KMEANS)/kmeans.o  $(SILHOUETTE)/silhouette.o $(RW)/rw.o -lm

mall_kmeans.o: mall_kmeans.c
	gcc -c mall_kmeans.c

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
	rm mall_kmeans.o

