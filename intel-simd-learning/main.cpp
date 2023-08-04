#include <x86intrin.h>

#include <types.h>
#include <data.h>

#include <stdio.h>
#include <limits.h>

#define print_m128_register(v, type, format)                             \
printf("[");                                                             \
for (i32 vi = 0; vi < ((sizeof(v) / sizeof(type)) - 1); ++vi)            \
{                                                                        \
  printf("%" #format ", ", v.__##type[vi]);                              \
}                                                                        \
printf("%" #format "]\n", v.__##type[((sizeof(v) / sizeof(type)) - 1)]); \

#define print_m128_register_int(v, type)   print_m128_register(v, type, Id)
#define print_m128_register_float(v, type) print_m128_register(v, type, f)

internal u32 get_byte_alignment(void *__p)
{
  ptr_val p = (ptr_val) __p;

  ptr_val mask = ~0;
  u32 result = 0;

  for (i32 i = 0;
       i < 63; 
       ++i)
  {
    ptr_val masked_p = p & mask;
    if (p != masked_p)
    {
      result = (1 << (i - 1));
      break;
    }

    mask <<= 1;
  }

  return(result);
}

i32 main()
{
  m128 a = {};

  printf("size of m128: %d\n", sizeof(a));
  printf("pointer: 0x%p - %d byte-aligned\n", &a, get_byte_alignment(&a));

  print_m128_register_int(a, i8);
  print_m128_register_float(a, f32);

  return(0);
}
