#include <assert.h>

void f2(int *x2, int *y2) __CPROVER_assigns(*x2) __CPROVER_ensures(*x2 > 5)
{
  *x2 = 10;
}

void f3(int *x3, int *y3) __CPROVER_assigns(*x3) __CPROVER_ensures(*x3 > 100)
{
  *x3 = 101;
}

int main()
{
  int p = 1;
  int q = 2;

  for(int i = 0; i < 5; ++i)
  {
    if(p < 3)
    {
      f2(&p, &q);
    }
    else
    {
      f3(&p, &q);
    }
  }
  assert(p > 100);
  assert(q == 2);

  return 0;
}
