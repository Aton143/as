#ifndef TYPES_H
#include <stdint.h> 

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;

typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;

typedef uintptr_t ptr_val;

typedef float     f32;
typedef double    f64;

typedef i8        b8;
typedef i32       b32;
typedef i64       b64;

union m128 
{
  __m128 simd;

  i64    __i64[2];
  u64    __u64[2];

  f64    __f64[2];

  i32    __i32[4];
  u32    __u32[4];

  f32    __f32[4];

  u16    __u16[8];
  i16    __i16[8];

  u8     __u8[16];
  i8     __i8[16];
};

#define unused(variable) (void) (variable)

#define internal         static
#define local_persist    static
#define global           static
#define local_const      static const
#define global_const     static const
#define external         extern "C"

#define TYPES_H
#endif
