make -C src/housing-dataset/kmedoids/

OMP_NUM_THREADS=1 ./src/housing-dataset/kmedoids/out 20641 9 data/housing.csv 10

make clean -C src/housing-dataset/kmedoids/