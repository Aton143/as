#include <stdint.h>
#include <stdio.h>

// NOTE(antoniom): Sadly, even with LTCG, this isn't possible
// MASM doesn't offer /GL as an option and this listing proves just that :(

extern "C" uint64_t ReadCPUTimer(void);

int main()
{
  uint64_t Cycles = ReadCPUTimer();
  printf("%lld cycles\n", Cycles);
  return(0);
}
