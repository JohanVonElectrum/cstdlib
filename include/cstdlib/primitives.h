#pragma once

#define UNSIGNED_MIN_MAX(name, type) \
    constexpr type name##_MIN = 0; \
    constexpr type name##_MAX = (type)(~(type)0)

// Unsigned 8-bit integer
typedef unsigned char u8_t;
static_assert(sizeof(u8_t) == 1, "u8_t must be 1 byte");
UNSIGNED_MIN_MAX(U8, u8_t);

// Unsigned 16-bit integer
typedef unsigned short u16_t;
static_assert(sizeof(u16_t) == 2, "u16_t must be 2 bytes");
UNSIGNED_MIN_MAX(U16, u16_t);

// Unsigned 32-bit integer
typedef unsigned int u32_t;
static_assert(sizeof(u32_t) == 4, "u32_t must be 4 bytes");
UNSIGNED_MIN_MAX(U32, u32_t);

// Unsigned 64-bit integer
typedef unsigned long long int u64_t;
static_assert(sizeof(u64_t) == 8, "u64_t must be 8 bytes");
UNSIGNED_MIN_MAX(U64, u64_t);

// Unsigned 128-bit integer
typedef __uint128_t u128_t;
static_assert(sizeof(u128_t) == 16, "u128_t must be 16 bytes");
// Construct a new `u128_t` from two `u64_t`
#define U128(hi, lo) (((u128_t)(hi) << 64) | (u128_t)(lo))
UNSIGNED_MIN_MAX(U128, u128_t);

#define SIGNED_MIN_MAX(name, type) \
    constexpr type name##_MIN = (type)(((type)1) << ((sizeof(type) << 3) - 1)); \
    constexpr type name##_MAX = ~name##_MIN

// Signed 8-bit integer
typedef char i8_t;
static_assert(sizeof(i8_t) == 1, "i8_t must be 1 byte");
SIGNED_MIN_MAX(I8, i8_t);

// Signed 16-bit integer
typedef short i16_t;
static_assert(sizeof(i16_t) == 2, "i16_t must be 2 bytes");
SIGNED_MIN_MAX(I16, i16_t);

// Signed 32-bit integer
typedef int i32_t;
static_assert(sizeof(i32_t) == 4, "i32_t must be 4 bytes");
SIGNED_MIN_MAX(I32, i32_t);

// Signed 64-bit integer
typedef long long int i64_t;
static_assert(sizeof(i64_t) == 8, "i64_t must be 8 bytes");
SIGNED_MIN_MAX(I64, i64_t);

// Signed 128-bit integer
typedef __int128_t i128_t;
static_assert(sizeof(i128_t) == 16, "i128_t must be 16 bytes");
// Construct a new `i128_t` from two `u64_t`
#define I128(hi, lo) (((i128_t)(hi) << 64) | (i128_t)(lo))
SIGNED_MIN_MAX(I128, i128_t);

// Unsigned pointer-size integer
typedef __SIZE_TYPE__ usize_t;
UNSIGNED_MIN_MAX(USIZE, usize_t);
// Signed pointer-size integer
typedef __PTRDIFF_TYPE__ isize_t;
SIGNED_MIN_MAX(IPTRDIFF, isize_t);
static_assert(sizeof(usize_t) == sizeof(isize_t), "usize_t and isize_t must be the same size");
static_assert(sizeof(usize_t) == sizeof(void*), "usize_t must be the same size as a pointer");

#undef UNSIGNED_MIN_MAX
#undef SIGNED_MIN_MAX

// 8-bit boolean
typedef u8_t b8_t;
// 32-bit boolean
typedef u32_t b32_t;

// 32-bit IEEE-754 floating point number
typedef float f32_t;
static_assert(sizeof(f32_t) == 4, "f32_t must be 4 bytes");
// 64-bit IEEE-754 floating point number
typedef double f64_t;
static_assert(sizeof(f64_t) == 8, "f64_t must be 8 bytes");
