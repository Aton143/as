#include <stdio.h>
#include <stdint.h>

extern "C" int64_t arithmetic_asm(int64_t *a);

int main()
{
  int64_t a = 0x1000;
  int64_t result = arithmetic_asm(&a);
  printf("%llx\n", result);
  return(0);
}
