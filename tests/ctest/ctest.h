#pragma once

#include <cstdlib/common.h>
#include <cstdlib/console.h>
#include <cstdlib/cstdlib.h>

#define test(name, ...) static INLINE b8_t test_##name(void) { print_cstr(#name "..."); __VA_ARGS__; println_cstr(" OK"); return true; }
#define run(...) int main() { cstdlib_init(); FOR_EACH($0$run, __VA_ARGS__) return 0; }
#define $0$run(name) if (!test_##name()) return 1;

#define assert_eq(lhs, rhs) if (lhs != rhs) { println_cstr(" FAIL"); println_cstr("\t" #lhs " != " #rhs); return false; }
