#include <time.h>
#include <string>
#include <cstdio>
#include <omp.h>
#include <iostream>
#include <algorithm>
#include <mutex>

int main(int argc, char** argv)
{
    if (argc < 2){
        printf("Missing number of elements\n");
        return -1;
    }
    if (argc < 3){
        printf("Missing number of Histogram Bins\n");
        return -1;
    }
    // printf("%d\n", std::atoi(argv[1]));
    int n = atoi(argv[1]);
    double data[n];
    for(int i = 0; i < n; ++i) data[i] = (double)i;
    double min_data = 0;
    double max_data = n-1;
    int bins = atoi(argv[2]);
    int histogram[bins];
    std::fill(histogram, histogram + bins, 0);
    double range = (max_data - min_data) / (double)bins;
    int numdiv = n / 4;
    omp_set_num_threads(4);

#pragma omp parallel reduction(+: histogram)
{
    int priv_min = omp_get_thread_num() * numdiv;
    int priv_max = priv_min + numdiv - 1;
    if (omp_get_thread_num() == 3){
        priv_max = n - 1;
    }
    for(int i = priv_min; i <= priv_max; ++i){
#pragma omp critical
        ++ histogram[int((data[i] - min_data)/ range)];
    }
};
    for (int j = 0; j < bins; ++j) {
        printf("%d ", histogram[j]);
    }
    printf("\n");

}
