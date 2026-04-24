#include <cstdlib/mem.h>

#include <windows.h>

static HANDLE hHeap;
static SYSTEM_INFO info;

b8_t mem_init(void) {
    GetSystemInfo(&info);
    hHeap = GetProcessHeap();
    return hHeap != NULL;
}

void* mem_heap_alloc(const usize_t size) {
    return HeapAlloc(hHeap, 0, size);
}

void* mem_heap_calloc(const usize_t num, const usize_t size) {
    return HeapAlloc(hHeap, HEAP_ZERO_MEMORY, num * size);
}

void* mem_heap_realloc(void* ptr, const usize_t new_size) {
    return HeapReAlloc(hHeap, HEAP_ZERO_MEMORY, ptr, new_size);
}

void mem_heap_free(void* ptr) {
    HeapFree(hHeap, 0, ptr);
}

static INLINE DWORD get_protection(const u8_t prot) {
    const b8_t read = prot & MEM_PROT_READ;
    const b8_t write = prot & MEM_PROT_WRITE;
    const b8_t execute = prot & MEM_PROT_EXEC;
    return read
               ? write
                     ? execute
                           ? PAGE_EXECUTE_READWRITE
                           : PAGE_READWRITE
                     : execute
                           ? PAGE_EXECUTE_READ
                           : PAGE_READONLY
               : write
                     // We can't have WRITEONLY. We force READWRITE.
                     ? execute
                           ? PAGE_EXECUTE_READWRITE
                           : PAGE_READWRITE
                     : execute
                           ? PAGE_EXECUTE
                           : PAGE_NOACCESS;
}

void* mem_virtual_reserve(const usize_t size) {
    return VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_NOACCESS);
}

void* mem_virtual_commit(void* addr, const usize_t size, const u8_t prot) {
    return VirtualAlloc(addr, size, MEM_COMMIT, get_protection(prot));
}

b8_t mem_virtual_decommit(void* addr, const usize_t size) {
    return VirtualFree(addr, size, MEM_DECOMMIT) != 0;
}

b8_t mem_virtual_protect(void* addr, const usize_t size, const u8_t prot) {
    DWORD old;
    return VirtualProtect(addr, size, get_protection(prot), &old) != 0;
}

b8_t mem_virtual_release(void* addr, const usize_t size) {
    return VirtualFree(addr, 0, MEM_RELEASE) != 0;
}

usize_t mem_get_page_size(void) {
    return info.dwPageSize;
}
