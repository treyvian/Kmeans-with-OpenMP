POINT = ../point/
KMEANS = ../k-means/
KMEDOIDS = ../k-medoids/
SILHOUETTE = ../silhouette-score/
RW = ../rw-csv/

all: out

out: mall.o $(POINT)point.o $(KMEANS)kmeans.o $(KMEDOIDS)kmedoids.o $(SILHOUETTE)silhouette.o $(RW)rw.o
	gcc -fopenmp -o out mall.o $(POINT)point.o $(KMEANS)kmeans.o $(KMEDOIDS)kmedoids.o $(SILHOUETTE)silhouette.o $(RW)rw.o -lm

mall.o: mall.c
	gcc -c mall.c

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
	rm mall.o

