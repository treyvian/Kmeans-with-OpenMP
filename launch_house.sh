make -C src/housing-dataset

OMP_NUM_THREADS=1 ./src/housing-dataset/out 20641 9 data/housing.csv

make clean -C src/housing-dataset