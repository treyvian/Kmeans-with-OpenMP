#ifndef KMEDOIDS_H

#define KMEDOIDS_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

#include "../point/point.h"

void k_medoids (point *, 
                        const int,  
                        const int);

#endif