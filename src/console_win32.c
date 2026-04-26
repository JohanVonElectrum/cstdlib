#include <cstdlib/common.h>
#include <cstdlib/console.h>
#include <cstdlib/mem.h>
#include <cstdlib/string.h>

#include <windows.h>

static HANDLE stdout;

b8_t cstdlib_console_init(void) {
    stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    return stdout != NULL && stdout != INVALID_HANDLE_VALUE;
}

static INLINE void print_cstr_len(const char* const msg, const usize_t len) {
    DWORD written;
    WriteConsoleA(stdout, msg, len, &written, NULL);
}

void print_cstr(const char* const msg) {
    print_cstr_len(msg, cstr_len(msg));
}

void println_cstr(const char* const msg) {
    const usize_t len = cstr_len(msg);
    char* buff = mem_heap_alloc(len + 3); // TODO: allocator?
    mem_copy(buff, msg, len);
    buff[len] = '\r';
    buff[len + 1] = '\n';
    buff[len + 2] = '\0';
    print_cstr_len(buff, len + 2);
    mem_heap_free(buff);
}
