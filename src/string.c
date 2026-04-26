#include <cstdlib/string.h>

usize_t cstr_len(const char* const str) {
    usize_t len = 0;
    while (str[len] != '\0') ++len;
    return len;
}
