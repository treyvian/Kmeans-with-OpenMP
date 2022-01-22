POINT = ../point/
RW = ../rw-csv/
OBJ_FILES = house.o $(POINT)point.o $(RW)rw.o

all: out

out: $(OBJ_FILES)
	gcc -o out $(OBJ_FILES) -lm -fopenmp

house.o: house.c
	gcc -c house.c -fopenmp

point.o: 
	make -C $(POINT)

rw.o: 
	make -C $(RW)


clean:
	rm out
	rm house.o
	make clean -C $(POINT)
	make clean -C $(RW)
