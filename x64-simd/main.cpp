#include <x86intrin.h>

#include <types.h>
#include <data.h>

#include <stdio.h>
#include <string.h>
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

#define call_printi(v, type, fn, vars) \
v.simdi = fn vars; \
printf(#fn #vars "\n"); print_simd_register_int(v, type);

#define call_printf(v, type, fn, vars) \
v.simd_##type = fn vars; \
printf(#fn #vars "\n"); print_simd_register_float(v, type);

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

i32 main(i32 arg_count, char **arg_values)
{
  b32 print_sse = false;
  b32 print_set = false;

  for (i32 i = 1;
       i < arg_count;
       ++i)
  {
    char *cur_arg = arg_values[i];
    if (strcmp("--sse", cur_arg) == 0)
    {
      print_sse = true;
    }
    else if (strcmp("--print_set", cur_arg) == 0)
    {
      print_set = true;
    }
  }

  m128 a128; a128.simd_f32 = _mm_setzero_ps();
  m128 b128; b128.simd_f32 = _mm_setzero_ps();
  m128 c128; c128.simd_f32 = _mm_setzero_ps();

  printf("size of m128: %d\n", sizeof(a128));
  printf("pointer: 0x%p - %d byte-aligned\n", &a128, get_byte_alignment(&a128));
  printf("compiler-returned alignment: %d\n", __alignof__(a128));

  if (print_sse)
  {
    printf("\nSSE:");

    if (print_set)
    {
      {
        printf("\n******************************************************************\n");
        printf("set integers:\n");
        printf("8-bit components:\n");

        call_printi(a128, i8,
                    _mm_set_epi8, (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
        call_printi(b128, i8,
                    _mm_set_epi8, (15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0));
        call_printi(a128, i8,
                    _mm_setr_epi8, (0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
        call_printi(a128, i8, _mm_set1_epi8, (255));

        printf("\n16-bit components:\n");

        call_printi(a128, i16, _mm_set_epi16, (0, 1, 2, 3, 4, 5, 6, 7));
        call_printi(b128, i16, _mm_set_epi16, (7, 6, 5, 4, 3, 2, 1, 0));
        call_printi(a128, i16, _mm_setr_epi16, (0, 1, 2, 3, 4, 5, 6, 7));
        call_printi(a128, i16, _mm_set1_epi16, (127));

        printf("\n32-bit components:\n");

        call_printi(a128, i32, _mm_set_epi32, (0, 1, 2, 3));
        call_printi(b128, i32, _mm_set_epi32, (3, 2, 1, 0));
        call_printi(a128, i32, _mm_setr_epi32, (0, 1, 2, 3));
        call_printi(a128, i32, _mm_set1_epi32, (63));

        printf("\n64-bit components:\n");
        call_printi(a128, i64, _mm_set_epi64, (m64(0), m64(1)));
        call_printi(b128, i64, _mm_set_epi64, (m64(1), m64(0)));
        call_printi(a128, i64, _mm_setr_epi64, (m64(0), m64(1)));
        call_printi(a128, i64, _mm_set1_epi64, (m64(63)));

        printf("\n");

        call_printi(a128, i64, _mm_set_epi64x, (0, 1));
        call_printi(b128, i64, _mm_set_epi64x, (1, 0));
        call_printi(a128, i64, _mm_set1_epi64x, (63));

        printf("\n");

        call_printi(a128, i8, _mm_setzero_si128, ());
      }

      {
        printf("\n******************************************************************\n");
        printf("set floating-points:\n");

        call_printf(a128, f32, _mm_set_ps, (0.0, 1.0, 2.0, 3.0));
        call_printf(b128, f32, _mm_set_ps, (3.0, 2.0, 1.0, 0.0));
        call_printf(a128, f32, _mm_setr_ps, (0.0, 1.0, 2.0, 3.0));

        printf("\n");

        call_printf(a128, f32, _mm_set_ps1, (2.0));
        call_printf(b128, f32, _mm_set1_ps, (1.0));

        printf("\n");

        call_printf(a128, f32, _mm_set_ss, (1.625f));

        printf("\n");

        call_printf(a128, f64, _mm_set_pd, (0.0, 1.0));
        call_printf(b128, f64, _mm_set_pd, (1.0, 0.0));
        call_printf(a128, f64, _mm_setr_pd, (0.0, 1.0));

        printf("\n");

        call_printf(a128, f64, _mm_set_pd1, (3.0));
        call_printf(a128, f64, _mm_set1_pd, (4.0));

        printf("\n");

        call_printf(a128, f64, _mm_set_sd, (1.625));

        printf("\n");

        call_printf(a128, f32, _mm_setzero_ps, ());
        call_printf(a128, f64, _mm_setzero_pd, ());
      }
    }
  }

  return(0);
}
