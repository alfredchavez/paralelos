#include <cstdio>
#include <mpi.h>
#include <cstdlib>
#include <algorithm>

/*
void multiplication_matrix_vector(
        double *local_A,
        double *local_x,
        double *local_y,
        int local_m,
        int n,
        int local_n,
        MPI_Comm comm, int numsize) {
    double *x;
    int local_i,j;
    x = (double*)malloc(n * numsize * sizeof(double));
    MPI_Allgather(local_x, local_n, MPI_DOUBLE, x, local_n ,MPI_DOUBLE, comm);
    for (local_i = 0; local_i < local_m; local_i++) {
        local_y[local_i] = 0.0;
        for (j = 0; j < n; j++) {
            local_y[local_i] += local_A[local_i * n + j] * x[j];
        }
    }
    free(x);
}

int main(int argc, char** argv) {
    int n,m, counter, numsize, myrank;
    double *matrix;
    double *vector;
    double *ans;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    n = 3; m = 3;
    matrix = new double[n * m];
    for (int i = 0; i < n * m; ++i) matrix[i] = 1;
    vector = new double[n];
    for (int i = 0; i < n; ++i) vector[i] = 2;
    ans = new double[m];
    counter = n;

    multiplication_matrix_vector(matrix, vector, ans, m, n, counter, MPI_COMM_WORLD, numsize);

    MPI_Finalize();
    if (myrank == 0) {
        for (int i = 0; i < m; ++i) {
            printf("%lf ", ans[i]);
        }
        printf("\n");
    }
    return 0;
}*/

int main(int argc, char** argv){
    int n, m, num_proc, myrank;
    int* matrix;
    int* vector;
    int* ans;
    int* displs;
    int* sendcounts;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    int DIM_SIZE = num_proc;
    int vector_size = DIM_SIZE;
    int matrix_size = DIM_SIZE * DIM_SIZE;
    if(myrank == 0){
        matrix = new int[matrix_size];
        vector = new int[vector_size];
        ans = new int[DIM_SIZE];
        //displs = new int[num_proc];
        //sendcounts = new int[num_proc];
        std::fill(matrix, matrix + matrix_size, 1);
        std::fill(vector, vector + vector_size, 2);
        std::fill(ans, ans + DIM_SIZE, 0);
        //std::fill(displs, displs + num_proc, 0);
        //std::fill(sendcounts, sendcounts + num_proc, 1);
    }
    int num_val = 1;
    int* recvbuf = new int[num_val];
    int* recvmatrix = new int[num_val];
    MPI_Scatter(vector,num_val, MPI_INT, recvbuf, num_val, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(matrix,DIM_SIZE, MPI_INT, recvmatrix, DIM_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    int val = 0;
    for (int k = 0; k < DIM_SIZE; ++k) {
        val += recvmatrix[k] * recvbuf[0];
    }
    int* valarr = new int[1];
    valarr[0] = val;
    for (int j = 0; j < num_val; ++j) {
        printf("%d -> %d ", myrank, val);
    }
    //printf("\n");
    MPI_Gather(valarr,1,MPI_INT,ans,1,MPI_INT,0,MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();
    if (myrank == 0){
        for (int i = 0; i < DIM_SIZE; ++i) {
            printf("%d ", ans[i]);
        }
        printf("\n");
        delete[] vector;
        delete[] matrix;
        delete[] displs;
        delete[] ans;
    }
    return 0;
}
