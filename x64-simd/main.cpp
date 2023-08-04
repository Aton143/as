#include <x86intrin.h>

#include <types.h>
#include <data.h>

#include <stdio.h>
#include <limits.h>

#define print_simd_register(v, type, format)                             \
printf("[");                                                             \
for (i32 vi = 0; vi < ((sizeof(v) / sizeof(type)) - 1); ++vi)            \
{                                                                        \
  printf("%" #format ", ", v.__##type[vi]);                              \
}                                                                        \
printf("%" #format "]\n", v.__##type[((sizeof(v) / sizeof(type)) - 1)]); \

#define print_simd_register_int(v, type)   print_simd_register(v, type, Id)
#define print_simd_register_float(v, type) print_simd_register(v, type, f)
#define call_print(fn, vars) fn vars; printf(#fn #vars "\n");

internal u64 get_byte_alignment(void *__p)
{
  ptr_val p = (ptr_val) __p;

  u64 result = 1;

  ptr_val mask     = ~0;
  ptr_val masked_p = p & mask;

  while (masked_p == p)
  {
    result   <<= 1;

    mask     <<= 1;
    masked_p   = p & mask;
  }

  return(result >> 1);
}

i32 main()
{
  m128 a128 = {};
  m128 b128 = {};
  m128 c128 = {};

  printf("size of m128: %d\n", sizeof(a128));
  printf("pointer: 0x%p - %d byte-aligned\n", &a128, get_byte_alignment(&a128));
  printf("compiler-returned alignment: %d\n", __alignof__(a128));

  printf("\nSSE:\n");

  printf("set:\n");

  a128.simdi = call_print(_mm_set_epi16, (0, 1, 2, 3, 4, 5, 6, 7));
  print_simd_register_int(a128, i16);

  b128.simdi = call_print(_mm_set_epi16, (7, 6, 5, 4, 3, 2, 1, 0));
  print_simd_register_int(b128, i16);

  return(0);
}
