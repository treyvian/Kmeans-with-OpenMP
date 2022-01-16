POINT = ../../point/
KMEDOIDS = ../../k-medoids/
SILHOUETTE = ../../silhouette-score/
RW = ../../rw-csv/

all: out

out: mall_kmedoids.o $(POINT)/point.o $(KMEDOIDS)/kmedoids.o  $(SILHOUETTE)/silhouette.o $(RW)/rw.o
	gcc -fopenmp -o out mall_kmedoids.o $(POINT)/point.o $(KMEDOIDS)/kmedoids.o  $(SILHOUETTE)/silhouette.o $(RW)/rw.o -lm

mall_kmedoids.o: mall_kmedoids.c
	gcc -c mall_kmedoids.c

point.o: 
	make -C $(POINT)

kmedoids.o: 
	make -C $(KMEDOIDS)

silhouette.o: 
	make -C $(SILHOUETTE)

rw.o: 
	make -C $(RW)


clean:
	rm out
	rm mall_kmedoids.o

