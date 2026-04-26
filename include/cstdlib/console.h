#pragma once

#include <cstdlib/api.h>
#include <cstdlib/primitives.h>

CSTDLIB_EXPORT b8_t cstdlib_console_init(void);

CSTDLIB_EXPORT void print_cstr(const char* msg);
CSTDLIB_EXPORT void println_cstr(const char* msg);
