#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

const int RMAX = 1000000;

int thread_count;

void Usage(char* prog_name);
void Get_args(int argc, char* argv[], int* n_p);
void Generate_list(int a[], int n);
void Odd_even(int a[], int n);

int main(int argc, char* argv[]) {
   int  n;
   char g_i;
   int* a;
   double start, finish;
   Get_args(argc, argv, &n);
   a = malloc(n*sizeof(int));
   Generate_list(a, n);
   start = omp_get_wtime();
   Odd_even(a, n);
   finish = omp_get_wtime();
   printf("%lf\n", finish - start);
   free(a);
   return 0;
}


void Get_args(int argc, char* argv[], int* n_p) {
   thread_count = strtol(argv[1], NULL, 10);
   *n_p = strtol(argv[2], NULL, 10);
}


void Generate_list(int a[], int n) {
   int i;

   srandom(1);
   for (i = 0; i < n; i++)
      a[i] = random() % RMAX;
}

void Odd_even(int a[], int n) {
   int phase, i, tmp;

   for (phase = 0; phase < n; phase++) {
      if (phase % 2 == 0)
#        pragma omp parallel for num_threads(thread_count) \
            default(none) shared(a, n) private(i, tmp)
         for (i = 1; i < n; i += 2) {
            if (a[i-1] > a[i]) {
               tmp = a[i-1];
               a[i-1] = a[i];
               a[i] = tmp;
            }
         }
      else
#        pragma omp parallel for num_threads(thread_count) \
            default(none) shared(a, n) private(i, tmp)
         for (i = 1; i < n-1; i += 2) {
            if (a[i] > a[i+1]) {
               tmp = a[i+1];
               a[i+1] = a[i];
               a[i] = tmp;
            }
         }
   }
}

