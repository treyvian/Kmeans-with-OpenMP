POINT = ../point/
RW = ../rw-csv/

OBJ_FILES = mall.o $(POINT)point.o $(RW)rw.o

all: out

out: $(OBJ_FILES)
	gcc -o out $(OBJ_FILES) -lm -fopenmp

mall.o: mall.c
	gcc -c mall.c -fopenmp

point.o: 
	make -C $(POINT)

rw.o: 
	make -C $(RW)


clean:
	rm out
	rm mall.o
	point.o: 
	make clean -C $(POINT)
	make clean -C $(RW)

