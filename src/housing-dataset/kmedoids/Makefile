POINT = ../../point/
KMEDOIDS = ../../k-medoids/
SILHOUETTE = ../../silhouette-score/
RW = ../../rw-csv/

all: out

out: housing_kmedoids.o $(POINT)/point.o $(KMEDOIDS)/kmedoids.o  $(SILHOUETTE)/silhouette.o $(RW)/rw.o
	gcc -fopenmp -o out housing_kmedoids.o $(POINT)/point.o $(KMEDOIDS)/kmedoids.o  $(SILHOUETTE)/silhouette.o $(RW)/rw.o -lm

housing_kmedoids.o: housing_kmedoids.c
	gcc -c housing_kmedoids.c

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
	rm housing_kmedoids.o

