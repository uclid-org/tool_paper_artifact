#include <stdlib.h>

int f1(int *a, int *b) __CPROVER_assigns(*a)
{
  while(*a > 0)
  {
    int *b = (int *)malloc(sizeof(int));
    *b = 5;
  }
}

int main()
{
  int m = 4;
  int n = 3;
  f1(&m, &n);

  return 0;
}
