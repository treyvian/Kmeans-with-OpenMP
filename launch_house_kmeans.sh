make -C src/housing-dataset/kmeans

OMP_NUM_THREADS=1 ./src/housing-dataset/kmeans/out 20641 9 data/housing.csv 10

make clean -C src/housing-dataset/kmeans