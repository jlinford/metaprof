#include <stdio.h>
#include <unistd.h>

#define DEFAULT_SIZE 1000000

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

  for (i=0; i<n; i++) {
    foo(i);
  }

  printf("Sleeping for 2 seconds...\n");
  usleep(2*1e6);

  printf("Done!\n");
  return 0;
}

