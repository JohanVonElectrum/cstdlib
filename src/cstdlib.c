#include <cstdlib/cstdlib.h>

#include <cstdlib/console.h>
#include <cstdlib/mem.h>

b8_t cstdlib_init(void) {
    return cstdlib_console_init() && cstdlib_mem_init();
}
