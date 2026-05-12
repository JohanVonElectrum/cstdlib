#pragma once

#if defined(_MSC_VER)
#include <sal.h>
#endif

#if defined(__clang__)
#define NONNULL          _Nonnull
#define NULLABLE         _Nullable
#define NULL_UNSPECIFIED _Null_unspecified
#else
#define NONNULL
#define NULLABLE
#define NULL_UNSPECIFIED
#endif

#if defined(__clang__) || defined(__GNUC__)
#define RETURNS_NONNULL  __attribute__((returns_nonnull))
#else
#define RETURNS_NONNULL
#endif

#if (defined(__GNUC__) || defined(__clang__)) && !defined(_MSC_VER)
#define NONNULL_ARGS(...) __attribute__((nonnull __VA_OPT__((__VA_ARGS__))))
#else
#define NONNULL_ARGS(...)
#endif

#if defined(__clang__)
#define ASSUME_NONNULL_BEGIN   _Pragma("clang assume_nonnull begin")
#define ASSUME_NONNULL_END     _Pragma("clang assume_nonnull end")
#else
#define ASSUME_NONNULL_BEGIN
#define ASSUME_NONNULL_END
#endif

#if defined(_MSC_VER)
#define IN_NONNULL     _In_
#define IN_NULLABLE    _In_opt_
#define OUT_NONNULL    _Out_
#define OUT_NULLABLE   _Out_opt_
#define INOUT_NONNULL  _Inout_
#define INOUT_NULLABLE _Inout_opt_
#define RET_NONNULL    _Ret_notnull_
#define RET_NULLABLE   _Ret_maybenull_
#else
#define IN_NONNULL     NONNULL
#define IN_NULLABLE    NULLABLE
#define OUT_NONNULL    NONNULL
#define OUT_NULLABLE   NULLABLE
#define INOUT_NONNULL  NONNULL
#define INOUT_NULLABLE NULLABLE
#define RET_NONNULL    RETURNS_NONNULL
#define RET_NULLABLE
#endif
