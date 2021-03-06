#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>

int     thread_count;
int     m, n;
double* A;
double* x;
double* y;

void Usage(char* prog_name);
void Read_matrix(char* prompt, double A[], int m, int n);
void Read_vector(char* prompt, double x[], int n);
void Print_matrix(char* title, double A[], int m, int n);
void Print_vector(char* title, double y[], double m);

void *Pth_mat_vect(void* rank);

int main(int argc, char* argv[]) {
   long       thread;
   pthread_t* thread_handles;

   if (argc != 2) Usage(argv[0]);
   thread_count = atoi(argv[1]);
   thread_handles = malloc(thread_count*sizeof(pthread_t));

   m = 8;
   n = 8000000;

   A = malloc(m*n*sizeof(double));
   x = malloc(n*sizeof(double));
   y = malloc(m*sizeof(double));
   
   Read_matrix("Enter the matrix", A, m, n);

   Read_vector("Enter the vector", x, n);
 
   double dtime = omp_get_wtime();

   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
         Pth_mat_vect, (void*) thread);

   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);
	
   dtime = omp_get_wtime() - dtime;   
   printf("%lf\n",dtime);

   free(A);
   free(x);
   free(y);

   return 0;
}  /* main */


void Usage (char* prog_name) {
   fprintf(stderr, "usage: %s <thread_count>\n", prog_name);
   exit(0);
}  /* Usage */

void Read_matrix(char* prompt, double A[], int m, int n) {
   int             i, j;

   for (i = 0; i < m; i++) 
      for (j = 0; j < n; j++)
         A[i*n+j] = 1;
}  /* Read_matrix */


void Read_vector(char* prompt, double x[], int n) {
   int   i;

   for (i = 0; i < n; i++) 
      x[i] = 0;
}  /* Read_vector */


void *Pth_mat_vect(void* rank) {
   long my_rank = (long) rank;
   int i, j;
   int local_m = m/thread_count; 
   int my_first_row = my_rank*local_m;
   int my_last_row = (my_rank+1)*local_m - 1;

   for (i = my_first_row; i <= my_last_row; i++) {
      y[i] = 0.0;
      for (j = 0; j < n; j++)
          y[i] += A[i*n+j]*x[j];
   }

   return NULL;
}  /* Pth_mat_vect */


void Print_matrix( char* title, double A[], int m, int n) {
   int   i, j;

   printf("%s\n", title);
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++)
         printf("%4.1f ", A[i*n + j]);
      printf("\n");
   }
}  /* Print_matrix */

void Print_vector(char* title, double y[], double m) {
   int   i;

   printf("%s\n", title);
   for (i = 0; i < m; i++)
      printf("%4.1f ", y[i]);
   printf("\n");
}  /* Print_vector */
