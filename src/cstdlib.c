#include <cstdlib/cstdlib.h>

#include <cstdlib/alloc.h>
#include <cstdlib/console.h>
#include <cstdlib/mem.h>
#include <cstdlib/thread.h>

b8_t cstdlib_init(const allocator_t* allocator) {
    if (allocator == nullptr) allocator = &HEAP_ALLOCATOR;
    return cstdlib_console_init() && cstdlib_mem_init() && (setup_thread(allocator) != nullptr);
}
