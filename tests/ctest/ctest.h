#pragma once

#include <cstdlib/common.h>

#define test(name, ...) static INLINE b8_t test_##name(void) { __VA_ARGS__; return true; }
#define run(...) int main() { FOR_EACH($0$run, __VA_ARGS__) return 0; }
#define $0$run(name) if (!test_##name()) return 1;

#define assert_eq(lhs, rhs) if (lhs != rhs) return false;
