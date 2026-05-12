#include <cstdlib/alloc.h>
#include <cstdlib/mem.h>

struct arena_t
{
    void* start;
    void* end;
    void* current;
    void* commited;
};

b8_t arena_create(const usize_t capacity, arena_t* const out_arena) {
    const usize_t aligned_capacity = MEM_ALIGN_UP(capacity, mem_get_page_size());
    out_arena->start = mem_virtual_reserve(aligned_capacity);
    if (!out_arena->start) return false;
    out_arena->end = (u8_t*) out_arena->start + aligned_capacity;
    out_arena->current = out_arena->start;
    out_arena->commited = out_arena->start;
    return true;
}

arena_t* arena_create_intrusive(const usize_t capacity) {
    const usize_t aligned_capacity = MEM_ALIGN_UP(capacity + sizeof(arena_t), mem_get_page_size());
    void* raw = mem_virtual_reserve(aligned_capacity);
    if (!raw) return nullptr;
    if (!mem_virtual_commit(raw, sizeof(arena_t), MEM_PROT_READWRITE)) {
        mem_virtual_release(raw, aligned_capacity);
        return nullptr;
    }
    arena_t* arena = raw;
    arena->start = nullptr;
    arena->end = (u8_t*) raw + aligned_capacity;
    arena->current = (u8_t*) raw + sizeof(arena_t);
    arena->commited = (u8_t*) arena + mem_get_page_size();
    return arena;
}

b8_t arena_destroy(arena_t* const arena) {
    return arena->start
               ? mem_virtual_release(arena->start, (usize_t) arena->end - (usize_t) arena->start)
               : mem_virtual_release(arena, (usize_t) arena->end - (usize_t) arena->start + sizeof(arena_t));
}

void* arena_alloc(arena_t* const arena, const usize_t size, const usize_t alignment) {
    void* const current = MEM_PTR_ALIGN_UP(arena->current, alignment);
    void* const new_cur = (u8_t*) current + size;

    if (new_cur > arena->end) return nullptr;

    if (new_cur > arena->commited) {
        void* const new_commited = MEM_PTR_ALIGN_UP(new_cur, mem_get_page_size());
        if (!mem_virtual_commit(
            arena->commited, (usize_t) new_commited - (usize_t) arena->commited, MEM_PROT_READWRITE
        )) {
            return nullptr;
        }
        arena->commited = new_commited;
    }

    arena->current = new_cur;
    return current;
}

void* arena_calloc(arena_t* const arena, const usize_t count, const usize_t size, const usize_t alignment) {
    const usize_t total_size = count * size;
    void* ptr = arena_alloc(arena, total_size, alignment);
    if (ptr == nullptr) return nullptr;
    mem_set(ptr, 0, total_size);
    return ptr;
}

void $0$arena_reset(arena_t* const arena) {
    $0$arena_reset_to(arena, arena->start);
}

INLINE void $0$arena_reset_to(arena_t* const arena, void* const addr) {
    arena->current = addr;
}

b8_t arena_free(arena_t* const arena) {
    void* const from = MEM_PTR_ALIGN_UP(arena->current, mem_get_page_size());
    const usize_t size = (usize_t) arena->commited - (usize_t) from;
    if (size == 0) return true;
    if (!mem_virtual_decommit(from, size)) return false;
    arena->commited = from;
    return true;
}

void* heap_allocator_alloc(void* allocator, const usize_t size, const usize_t alignment) {
    return mem_heap_alloc(size);
}

void* heap_allocator_calloc(void* allocator, const usize_t count, const usize_t size, const usize_t alignment) {
    return mem_heap_calloc(count, size);
}

void* heap_allocator_realloc(void* allocator, void* ptr, const usize_t size, const usize_t alignment) {
    return mem_heap_realloc(ptr, size);
}

b8_t heap_allocator_free(void* allocator, void* ptr) {
    return mem_heap_free(ptr);
}

const allocator_t HEAP_ALLOCATOR = (allocator_t){
    .allocator = nullptr,
    .alloc = heap_allocator_alloc,
    .calloc = heap_allocator_calloc,
    .realloc = heap_allocator_realloc,
    .free = heap_allocator_free
};

allocator_t arena_allocator(arena_t* arena) {
    return (allocator_t){
        .allocator = arena,
        .alloc = (allocator_alloc_fn_t) arena_alloc,
        .calloc = (allocator_calloc_fn_t) arena_calloc,
        .realloc = nullptr,
        .free = nullptr
    };
}

void* $1$allocator_alloc(const usize_t size, const usize_t alignment, const allocator_t* const allocator) {
    if (allocator->alloc == nullptr) return nullptr;
    return allocator->alloc(allocator->allocator, size, alignment);
}

void* $1$allocator_calloc(const usize_t count, const usize_t size, const usize_t alignment, const allocator_t* const allocator) {
    if (allocator->calloc == nullptr) return nullptr;
    return allocator->calloc(allocator->allocator, count, size, alignment);
}
void* $1$allocator_realloc(void* const ptr, const usize_t size, const usize_t alignment, const allocator_t* const allocator) {
    if (allocator->realloc == nullptr) return nullptr;
    return allocator->realloc(allocator->allocator, ptr, size, alignment);
}

b8_t $1$allocator_free(void* const ptr, const allocator_t* const allocator) {
    if (allocator->free == nullptr) return false;
    return allocator->free(allocator->allocator, ptr);
}
