#include <stdio.h>
#include <unistd.h>
#include <omp.h>

#define DEFAULT_SIZE 100000000

int foo(int x) {
  int res; 
  res = x+3*x*x; 
  return res;
}

int main(int argc, char **argv) 
{
  int i, n;
  if (argc > 1) {
    sscanf(argv[1], "%d", &n);
  } else {
    n = DEFAULT_SIZE;
  }
  printf("%s: Using n = %d\n", argv[0], n);


  #pragma omp parallel 
  {
    printf("Thread %d/%d alive\n", omp_get_thread_num(), omp_get_num_threads());
    #pragma omp for
    for (i=0; i<n; i++) {
      foo(i);
    }
  }

  printf("Sleeping for 2 seconds...\n");
  usleep(2*1e6);

  printf("Done!\n");
  return 0;
}

