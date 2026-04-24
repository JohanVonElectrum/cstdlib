#pragma once

#include <cstdlib/primitives.h>

typedef struct arena_t arena_t;

b8_t arena_create(usize_t capacity, arena_t* out_arena);
arena_t* arena_create_intrusive(usize_t capacity);
b8_t arena_destroy(arena_t* arena);
void* arena_alloc(arena_t* arena, usize_t size, usize_t alignment);
void arena_reset(arena_t* arena);
void arena_reset_to(arena_t* arena, void* addr);
b8_t arena_free(arena_t* arena);

#define ARENA_RESET(arena, ...) arena_reset##__VA_OPT__(_to)(arena __VA_OPT__(,__VA_ARGS__))
