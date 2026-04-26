#include <cstdlib/string.h>

#include <cstdlib/mem.h>

struct str_t
{
    usize_t len;
};

const char* cstr_from(const str_t* const str) {
    return (char*)str + sizeof(str_t);
}

usize_t cstr_len(const char* const str) {
    usize_t len = 0;
    while (str[len] != '\0') ++len;
    return len;
}

INLINE const str_t* str_new(const char* const str, const usize_t len, const allocator_t* const allocator) {
    str_t* buff = allocator->alloc(allocator->allocator, sizeof(str_t) + len + 1, alignof(str_t));
    buff->len = len;
    char* cstr = (char*)buff + sizeof(str_t);
    mem_copy(cstr, str, len);
    cstr[len] = '\0';
    return buff;
}

const str_t* str_from(const char* const str, const allocator_t* const allocator) {
    return str_new(str, cstr_len(str), allocator);
}

void str_free(str_t* const str, const allocator_t* const allocator) {
    allocator->free(allocator->allocator, str);
}

usize_t str_len(const str_t* const str) {
    return str->len;
}
