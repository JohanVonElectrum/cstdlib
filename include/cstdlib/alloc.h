#pragma once

#include <cstdlib/api.h>
#include <cstdlib/primitives.h>

typedef struct arena_t arena_t;

CSTDLIB_EXPORT b8_t arena_create(usize_t capacity, arena_t* out_arena);
CSTDLIB_EXPORT arena_t* arena_create_intrusive(usize_t capacity);
CSTDLIB_EXPORT b8_t arena_destroy(arena_t* arena);
CSTDLIB_EXPORT void* arena_alloc(arena_t* arena, usize_t size, usize_t alignment);
CSTDLIB_EXPORT void arena_reset(arena_t* arena);
CSTDLIB_EXPORT void arena_reset_to(arena_t* arena, void* addr);
CSTDLIB_EXPORT b8_t arena_free(arena_t* arena);

#define ARENA_RESET(arena, ...) arena_reset##__VA_OPT__(_to)(arena __VA_OPT__(,__VA_ARGS__))

typedef struct allocator_t
{
    void* allocator;
    void* (*alloc)(void* allocator, usize_t size, usize_t alignment);
    void* (*realloc)(void* allocator, void* ptr, usize_t size, usize_t alignment);
    b8_t (*free)(void* allocator, void* ptr);
} allocator_t;

CSTDLIB_EXPORT extern const allocator_t HEAP_ALLOCATOR;
