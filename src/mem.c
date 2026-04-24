#include <cstdlib/mem.h>

static constexpr usize_t WORD_MASK = sizeof(usize_t) - 1;

void mem_copy(void* restrict dst, const void* restrict src, const usize_t count) {
    if (((usize_t) dst & WORD_MASK) || ((usize_t) src & WORD_MASK) || (count & WORD_MASK)) {
        u8_t* d = dst;
        const u8_t* s = src;

        for (usize_t i = 0; i < count; ++i) {
            d[i] = s[i];
        }
    } else {
        usize_t* d = dst;
        const usize_t* s = src;
        const usize_t words = count / sizeof(usize_t);

        for (usize_t i = 0; i < words; ++i) {
            d[i] = s[i];
        }
    }
}

void mem_move(void* dst, const void* src, const usize_t count) {
    const u8_t* const s8 = src;
    u8_t* const d8 = dst;

    if (s8 < d8 && d8 < s8 + count) {
        if (((usize_t) dst & WORD_MASK) || ((usize_t) src & WORD_MASK) || (count & WORD_MASK)) {
            for (usize_t i = count; i > 0; --i) {
                d8[i - 1] = s8[i - 1];
            }
        } else {
            usize_t* const d = dst;
            const usize_t* const s = src;
            const usize_t words = count / sizeof(usize_t);

            for (usize_t i = words; i > 0; --i) {
                d[i - 1] = s[i - 1];
            }
        }
    } else if (d8 < s8 && s8 < d8 + count) {
        if (((usize_t) dst & WORD_MASK) || ((usize_t) src & WORD_MASK) || (count & WORD_MASK)) {
            for (usize_t i = 0; i < count; ++i) {
                d8[i] = s8[i];
            }
        } else {
            usize_t* const d = dst;
            const usize_t* const s = src;
            const usize_t words = count / sizeof(usize_t);

            for (usize_t i = 0; i < words; ++i) {
                d[i] = s[i];
            }
        }
    } else {
        mem_copy(dst, src, count);
    }
}

void mem_set(void* dst, const u8_t value, const usize_t count) {
    const u8_t* const limit = (u8_t*) dst + count;
    for (u8_t* d = dst; d < limit; ++d) {
        *d = value;
    }
}

i16_t mem_cmp(const void* lhs, const usize_t lhs_count, const void* rhs, const usize_t rhs_count) {
    const u8_t* const lhs8 = lhs;
    const u8_t* const rhs8 = rhs;

    if (lhs_count < rhs_count) {
        for (usize_t i = 0; i < lhs_count; ++i) {
            const i16_t value = (i16_t) lhs8[i] - (i16_t) rhs8[i];
            if (value != 0) return value;
        }
        return -1;
    }
    for (usize_t i = 0; i < rhs_count; ++i) {
        const i16_t value = (i16_t) lhs8[i] - (i16_t) rhs8[i];
        if (value != 0) return value;
    }
    return (rhs_count < lhs_count) ? 1 : 0;
}

void* mem_chr(const void* ptr, const u8_t value, const usize_t count) {
    const u8_t* const limit = (u8_t*) ptr + count;
    for (u8_t* p = (u8_t*) ptr; p < limit; ++p) {
        if (*p == value) return p;
    }
    return nullptr;
}
