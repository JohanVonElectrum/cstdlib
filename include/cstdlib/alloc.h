#pragma once

#include <cstdlib/api.h>
#include <cstdlib/primitives.h>

typedef struct arena_t arena_t;

CSTDLIB_EXPORT b8_t arena_create(usize_t capacity, arena_t* out_arena);
CSTDLIB_EXPORT arena_t* arena_create_intrusive(usize_t capacity);
CSTDLIB_EXPORT b8_t arena_destroy(arena_t* arena);
CSTDLIB_EXPORT void* arena_alloc(arena_t* arena, usize_t size, usize_t alignment);
CSTDLIB_EXPORT void* arena_calloc(arena_t* arena, usize_t count, usize_t size, usize_t alignment);
#define arena_reset(arena, ...) $0$arena_reset##__VA_OPT__(_to)(arena __VA_OPT__(,__VA_ARGS__))
CSTDLIB_EXPORT void $0$arena_reset(arena_t* arena);
CSTDLIB_EXPORT void $0$arena_reset_to(arena_t* arena, void* addr);
CSTDLIB_EXPORT b8_t arena_free(arena_t* arena);

typedef void*(*allocator_alloc_fn_t)(void* allocator, usize_t size, usize_t alignment);
typedef void*(*allocator_calloc_fn_t)(void* allocator, usize_t count, usize_t size, usize_t alignment);
typedef void*(*allocator_realloc_fn_t)(void* allocator, void* ptr, usize_t size, usize_t alignment);
typedef b8_t(*allocator_free_fn_t)(void* allocator, void* ptr);

typedef struct allocator_t
{
    void* allocator;
    allocator_alloc_fn_t alloc;
    allocator_calloc_fn_t calloc;
    allocator_realloc_fn_t realloc;
    allocator_free_fn_t free;
} allocator_t;

CSTDLIB_EXPORT extern const allocator_t HEAP_ALLOCATOR;

CSTDLIB_EXPORT allocator_t arena_allocator(arena_t* arena);

#define allocator_alloc(T, ...) $0$allocator_alloc(T, __VA_OPT__(__VA_ARGS__,) &HEAP_ALLOCATOR)
#define $0$allocator_alloc(T, allocator, ...) $1$allocator_alloc(sizeof(T), alignof(T), allocator)
CSTDLIB_EXPORT void* $1$allocator_alloc(usize_t size, usize_t alignment, const allocator_t* allocator);

#define allocator_calloc(count, T, ...) $0$allocator_calloc(count, T, __VA_OPT__(__VA_ARGS__,) &HEAP_ALLOCATOR)
#define $0$allocator_calloc(count, T, allocator, ...) $1$allocator_calloc(count, sizeof(T), alignof(T), allocator)
CSTDLIB_EXPORT void* $1$allocator_calloc(usize_t count, usize_t size, usize_t alignment, const allocator_t* allocator);

#define allocator_realloc(ptr, T, ...) $0$allocator_realloc(ptr, T, __VA_OPT__(__VA_ARGS__,) &HEAP_ALLOCATOR)
#define $0$allocator_realloc(ptr, T, allocator, ...) $1$allocator_realloc(ptr, sizeof(T), alignof(T), allocator)
CSTDLIB_EXPORT void* $1$allocator_realloc(void* ptr, usize_t size, usize_t alignment, const allocator_t* allocator);

#define allocator_free(ptr, ...) $0$allocator_free(ptr, __VA_OPT__(__VA_ARGS__,) &HEAP_ALLOCATOR)
#define $0$allocator_free(ptr, allocator, ...) $1$allocator_free(ptr, allocator)
CSTDLIB_EXPORT b8_t $1$allocator_free(void* ptr, const allocator_t* allocator);
