make -C src/housing-dataset

# If you want to use valgrind
# Keep in mind it will take a lot of time
# OMP_NUM_THREADS=1 valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./src/housing-dataset/out 20641 9 data/housing.csv

OMP_NUM_THREADS=1 ./src/housing-dataset/out 20641 9 data/housing.csv

make clean -C src/housing-dataset