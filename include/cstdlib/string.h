#pragma once

#include <cstdlib/alloc.h>
#include <cstdlib/api.h>
#include <cstdlib/primitives.h>

typedef struct str_t str_t;

CSTDLIB_EXPORT const char* cstr_from(const str_t* str);
CSTDLIB_EXPORT usize_t cstr_len(const char* str);

CSTDLIB_EXPORT const str_t* str_new(const char* str, usize_t len, const allocator_t* allocator);
CSTDLIB_EXPORT const str_t* str_from(const char* str, const allocator_t* allocator);
CSTDLIB_EXPORT void str_free(str_t* str, const allocator_t* allocator);
CSTDLIB_EXPORT usize_t str_len(const str_t* str);
