# Compiling with make file
make -C src/mall-costumer-dataset

OMP_NUM_THREADS=1 valgrind --leak-check=full --show-leak-kinds=all -s ./src/mall-costumer-dataset/out 201 5 data/Mall_Customers.csv 10 
#OMP_NUM_THREADS=1 ./src/mall-costumer-dataset/out 201 5 data/Mall_Customers.csv 10

make clean -C src/mall-costumer-dataset