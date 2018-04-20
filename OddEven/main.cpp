#include <cstdio>
#include <mpi.h>
#include <cstdlib>
#include <algorithm>

int main(int argc, char** argv){
    int n, m, num_proc, myrank;
    int* vector;
    int* sendcounts;
    int* dsp2;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    int DIM_SIZE = num_proc * 2;
    int vector_size = DIM_SIZE + 1;
    if(myrank == 0){
        vector = new int[vector_size];
        dsp2 = new int[vector_size + 1];
        sendcounts = new int[vector_size + 1];
        std::fill(vector, vector + vector_size, 2);
        for (int i = 0; i < vector_size; ++i) {
            vector[i] = 10 - i;
        }

        for (int i = 0; i < DIM_SIZE; ++i) {
            printf("%d ", vector[i]);
        }
        printf("\n");
        vector[vector_size - 1] = 100;
        std::fill(dsp2, dsp2 + vector_size, 0);
        std::fill(sendcounts, sendcounts + vector_size, 2);
        for (int j = 0; j < vector_size; ++j) {
            dsp2[j] = 2 * j + 1;
        }
    }
    int num_val = 2;
    int* recvbuf = new int[num_val];

    for (int j = 0; j < DIM_SIZE + 1; ++j) {
        MPI_Scatter(vector, 2 ,MPI_INT,recvbuf,2,MPI_INT,0,MPI_COMM_WORLD);
        if (recvbuf[0] > recvbuf[1]){
            int tmp = recvbuf[1];
            recvbuf[1] = recvbuf[0];
            recvbuf[0] = tmp;
        }
        MPI_Gather(recvbuf, 2, MPI_INT, vector, 2, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatterv(vector,sendcounts,dsp2,MPI_INT,recvbuf,2,MPI_INT,0,MPI_COMM_WORLD);
        if (recvbuf[0] > recvbuf[1]){
            int tmp = recvbuf[1];
            recvbuf[1] = recvbuf[0];
            recvbuf[0] = tmp;
        }
        //printf("%d -> %d,%d\n", myrank, recvbuf[0], recvbuf[1]);
        MPI_Gatherv(recvbuf, 2, MPI_INT, vector, sendcounts, dsp2, MPI_INT,0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();
    if (myrank == 0){
        for (int i = 0; i < DIM_SIZE; ++i) {
            printf("%d ", vector[i]);
        }
        printf("\n");
        delete[] vector;
    }
    return 0;
}