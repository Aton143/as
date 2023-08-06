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

#define print_simd_register_int(v, type)   print_simd_register(v, type, llx)
#define print_simd_register_float(v, type) print_simd_register(v, type, f)

#define call_print(fn, vars) fn vars; printf(#fn #vars "\n");

#define call_printi(v, type, fn, vars) \
v.simdi = call_print(fn, vars); print_simd_register_int(v, type);

#define call_printf(v, type, fn, vars) \
v.simd_##type = call_print(fn, vars); print_simd_register_float(v, type);

#define array_count(array) ((sizeof(array)) / (sizeof(array[0])))
#define print_static_array(array, format)                  \
printf("[");                                               \
for (i32 i = 0; i < array_count(array) - 1; ++i)           \
{                                                          \
  printf("%" #format ", ", array[i]);                      \
}                                                          \
printf("%" #format "]\n", array[array_count(array) - 1]);

#define print_arrayf(array) print_static_array(array, .2f)
#define print_arrayi(array) print_static_array(array, llx)

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
  b32 print_sse   = false;
  b32 print_set   = false;
  b32 print_load  = false;
  b32 print_store = false;

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
    else if (strcmp("--print_load", cur_arg) == 0)
    {
      print_load = true;
    }
    else if (strcmp("--print_store", cur_arg) == 0)
    {
      print_store = true;
    }
  }

  m128 a128; a128.simd_f32 = _mm_setzero_ps();
  m128 b128; b128.simd_f32 = _mm_setzero_ps();
  m128 c128; c128.simd_f32 = _mm_setzero_ps();

  printf("size of m128: %d\n", sizeof(a128));
  printf("pointer: 0x%p - %d byte-aligned\n", &a128, get_byte_alignment(&a128));
  printf("compiler-returned alignment: %d\n", __alignof__(a128));

  printf("size of data: %d\n", sizeof(data_f32_0_4095));
  printf("pointer: 0x%p - %d byte-aligned\n",
         &data_f32_0_4095, get_byte_alignment(data_f32_0_4095));

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
    
    if (print_load)
    {
      printf("\n******************************************************************\n");
      printf("load:\n");

      call_printi(a128, i32, _mm_lddqu_si128, ((__m128i *) data_i32_0_4095));
      call_printi(a128, i64, _mm_load_si128, ((__m128i *) data_i32_0_4095));

      printf("\n");

      for (i32 offset = 0; offset < 4; ++offset)
      {
        call_printf(a128, f64, _mm_load1_pd, (data_f64_0_4095 + offset));
        call_printf(a128, f64, _mm_loaddup_pd, (data_f64_0_4095 + offset));
      }

      printf("\n");

      a128.simd_f64 = _mm_load_pd(data_f64_0_4095);
      call_printf(b128, f64, _mm_loadh_pd, (a128.simd_f64, data_f64_0_4095 + 3));
      call_printf(c128, f64, _mm_loadl_pd, (a128.simd_f64, data_f64_0_4095 + 3));

      printf("\n");

      a128.simd_f32 = _mm_loadu_ps(data_f32_0_4095 + 1);
      call_printf(b128, f32, _mm_loadh_pi, (a128.simd_f32, (__m64 *) (data_f32_0_4095 + 5)));
      call_printf(c128, f32, _mm_loadl_pi, (a128.simd_f32, (__m64 *) (data_f32_0_4095 + 5)));

      printf("\n");

      call_printi(a128, i32, _mm_stream_load_si128, ((__m128i *) data_i32_0_4095));
    }

    if (print_store)
    {
      printf("\n******************************************************************\n");
      printf("store:\n");

      local_persist i32    __attribute__((aligned(64))) i32_store[4]  = {};
      local_persist float  __attribute__((aligned(64))) f32_store[16] = {};
      local_persist double __attribute__((aligned(64))) f64_store[8]  = {};

      a128.simd_f32 = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
      call_print(_mm_store_ps, (f32_store, a128.simd_f32));
      print_arrayf(f32_store);

      memset(f32_store, 0, sizeof(f32_store));
      call_print(_mm_store1_ps, (f32_store, a128.simd_f32));
      print_arrayf(f32_store);

      printf("\n");

      memset(i32_store, 0, sizeof(i32_store));
      a128.simdi = _mm_set_epi8(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
      b128.simdi = _mm_set_epi8(0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1);
      call_print(_mm_maskmoveu_si128, (a128.simdi, b128.simdi, (char *) i32_store));

      a128.simdi = _mm_load_si128((__m128i *) i32_store);
      print_simd_register_int(a128, i8);
    }
  }

  return(0);
}
