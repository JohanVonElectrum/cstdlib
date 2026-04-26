#pragma once

#include <cstdlib/api.h>
#include <cstdlib/common.h>

CSTDLIB_EXPORT b8_t cstdlib_mem_init(void);

// ============================================================================
// Memory alignment utilities (low-level helpers used by allocators and VM)
// ============================================================================

#define MEM_IS_POWER_OF_TWO(x) (((x) != 0) && (((x) & ((x) - 1)) == 0))
#define MEM_ALIGN_UP(x, align) (((x) + (align) - 1) & ~((align) - 1))
#define MEM_ALIGN_DOWN(x, align) ((x) & ~((align) - 1))
#define MEM_IS_ALIGNED(x, align) (((x) & ((align) - 1)) == 0)

#define MEM_PTR_ALIGN_UP(ptr, align) ((void*)MEM_ALIGN_UP((usize_t)(ptr), (align)))
#define MEM_PTR_ALIGN_DOWN(ptr, align) ((void*)MEM_ALIGN_DOWN((usize_t)(ptr), (align)))

// ============================================================================
// General memory operations (abstracted equivalents of memcpy/memset/etc.)
// ============================================================================

CSTDLIB_EXPORT void mem_copy(void* restrict dst, const void* restrict src, usize_t count);
CSTDLIB_EXPORT void mem_move(void* dst, const void* src, usize_t count);
CSTDLIB_EXPORT void mem_set(void* dst, u8_t value, usize_t count);
CSTDLIB_EXPORT i16_t mem_cmp(const void* lhs, usize_t lhs_count, const void* rhs, usize_t rhs_count);
CSTDLIB_EXPORT void* mem_chr(const void* ptr, u8_t value, usize_t count);

static INLINE void mem_zero(void* dst, const usize_t count) {
    mem_set(dst, 0, count);
}

#define MEM_ZERO(ptr, size) mem_zero((ptr), (size))
#define MEM_ZERO_STRUCT(s)  mem_zero(&(s), sizeof(s))

// ============================================================================
// Heap memory (dynamic allocation)
// ============================================================================

CSTDLIB_EXPORT void* mem_heap_alloc(usize_t size);
CSTDLIB_EXPORT void* mem_heap_calloc(usize_t num, usize_t size);
CSTDLIB_EXPORT void* mem_heap_realloc(void* ptr, usize_t new_size);
CSTDLIB_EXPORT void mem_heap_free(void* ptr);

// ============================================================================
// Virtual memory (page-level reservation, commit, protection, etc.)
// ============================================================================

// Protection flags (bitmask)
constexpr u8_t MEM_PROT_NONE = 0;
constexpr u8_t MEM_PROT_READ = BIT(0);
constexpr u8_t MEM_PROT_WRITE = BIT(1);
constexpr u8_t MEM_PROT_EXEC = BIT(2);
constexpr u8_t MEM_PROT_READWRITE = MEM_PROT_READ | MEM_PROT_WRITE;

// Full virtual memory control (reserve/commit model - works on both POSIX and Win32)
CSTDLIB_EXPORT void* mem_virtual_reserve(usize_t size);
CSTDLIB_EXPORT void* mem_virtual_commit(void* addr, usize_t size, u8_t prot);
CSTDLIB_EXPORT b8_t mem_virtual_decommit(void* addr, usize_t size);
CSTDLIB_EXPORT b8_t mem_virtual_protect(void* addr, usize_t size, u8_t prot);
CSTDLIB_EXPORT b8_t mem_virtual_release(void* addr, usize_t size);

// Query platform page size (useful for alignment / large allocations)
CSTDLIB_EXPORT usize_t mem_get_page_size(void);
