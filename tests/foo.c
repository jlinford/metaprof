#include <stdio.h>

#define DEFAULT_SIZE 1000

int foo(int x) {
  int res; 
  TAU_START("infoo");
  res = x+3*x*x; 
  TAU_STOP("infoo");
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

  printf("Done!\n");
  return 0;
}
