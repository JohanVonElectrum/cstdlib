#include <windows.h>

#include <cstdlib/$mem.h>

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID lpReserved) {
    mem_init();
    return TRUE;
}
