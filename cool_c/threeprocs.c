#include <stdio.h>

a(int j, int *k)
{
  char **s;
  int i;

  s = (char **) &s;

  printf("a: &i = 0x%x, &j = 0x%x, &k = 0x%x\n", &i, &j, &k);

  for (i = 0; i < 100; i++) {
    s++;
    printf("0x%x : %15d 0x%-8x\n", s, *s, *s);
  }
}

b(int j)
{
  int i;

  j++;
  i = j+15;

  printf("b: &i = 0x%x, &j = 0x%x\n", &i, &j);

  a(49, &j);
}


main()
{
  int i;

  i = 333;

  b(i);
}

