#include "ctest.h"

#include <cstdlib/alloc.h>
#include <cstdlib/mem.h>
#include <cstdlib/string.h>

test(str, {
    const char* cstr = "Hello World!";
    const str_t* str = str_from(cstr, &HEAP_ALLOCATOR);
    const char* cstr2 = cstr_from(str);
    const usize_t len = str_len(str) + 1;
    assert_eq(mem_cmp(cstr, len, cstr2, len), 0);
})

run(str)
