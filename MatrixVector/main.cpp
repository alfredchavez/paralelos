#include <cstdio>
#include <mpi.h>
#include <cstdlib>

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
}