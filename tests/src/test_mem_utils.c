#include "ctest.h"

#include <cstdlib/mem.h>

test(mem_copy, {
    constexpr char src[14] = "Hello, world!";
    char dst[14];
    mem_copy(dst, src, 14);
    assert_eq(dst[0], 'H');
    assert_eq(dst[1], 'e');
    assert_eq(dst[2], 'l');
    assert_eq(dst[3], 'l');
    assert_eq(dst[4], 'o');
    assert_eq(dst[5], ',');
    assert_eq(dst[6], ' ');
    assert_eq(dst[7], 'w');
    assert_eq(dst[8], 'o');
    assert_eq(dst[9], 'r');
    assert_eq(dst[10], 'l');
    assert_eq(dst[11], 'd');
    assert_eq(dst[12], '!');
})

test(mem_move, {
    u8_t x[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const u8_t y = x[1];
    mem_move(x, &x[1], sizeof(x) - 1);
    assert_eq(x[0], y);
    assert_eq(x[sizeof(x) - 2], x[sizeof(x) - 1]);
})

test(mem_set, {
    u8_t x[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    mem_set(x, 0xFF, sizeof(x));
    for (usize_t i = 0; i < sizeof(x); ++i) {
        assert_eq(x[i], 0xFF);
    }
})

test(mem_cmp, {
    const char* const a = "Hello, world!";
    const char* const b = "Hello, world!";
    const char* const c = "Hello, world?";
    assert_eq(mem_cmp(a, 13, b, 13), 0);
    assert_eq(mem_cmp(a, 13, c, 13), -30);
    assert_eq(mem_cmp(c, 13, a, 13), 30);
})

test(mem_chr, {
    const char* const str = "Hello, world!";
    const char* const o = mem_chr(str, 'o', 13);
    const usize_t offset = o - str;
    assert_eq(offset, 4);
})

run(mem_copy, mem_move, mem_set, mem_cmp, mem_chr)
