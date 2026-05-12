#include <cstdlib/thread.h>

#include <process.h>
#include <windows.h>

#define THREAD_STATE_JOINABLE 0
#define THREAD_STATE_DETACHED 1
#define THREAD_STATE_FINISHED 2

struct thread_t
{
    const allocator_t* allocator;

    HANDLE handle;
    u32_t thread_id;

    thread_fn_t fn;
    void* param;
    void* retval;

    LONG state;
};

static thread_local thread_t* current_thread = nullptr;

thread_t* setup_thread(const allocator_t* const allocator) {
    if (current_thread != nullptr) return current_thread;

    current_thread = allocator_calloc(1, thread_t, allocator);
    if (current_thread == nullptr) return nullptr;

    current_thread->allocator = allocator;
    current_thread->handle = GetCurrentThread();
    current_thread->thread_id = GetCurrentThreadId();
    current_thread->state = THREAD_STATE_JOINABLE;

    return current_thread;
}

static unsigned __stdcall thread_wrapper(void* param) {
    thread_t* thread = param;
    current_thread = thread;

    thread->retval = thread->fn(thread->param);

    const LONG prev = InterlockedExchange(&thread->state, THREAD_STATE_FINISHED);
    if (prev == THREAD_STATE_DETACHED)
        allocator_free(thread, thread->allocator);

    _endthreadex(0);
    return 0;
}

thread_t* spawn_thread(const thread_fn_t fn, void* const param, const allocator_t* const allocator) {
    thread_t* thread = allocator_calloc(1, thread_t, allocator);
    if (thread == nullptr) return nullptr;

    thread->allocator = allocator;
    thread->fn = fn;
    thread->param = param;
    thread->state = THREAD_STATE_JOINABLE;

    thread->handle = (HANDLE) _beginthreadex(nullptr, 0, thread_wrapper, thread, 0, &thread->thread_id);
    if (thread->handle == nullptr) {
        allocator_free(thread, allocator);
        return nullptr;
    }

    return thread;
}

void exit_thread(const u32_t exit_code) {
    if (current_thread == nullptr) {
        ExitThread(exit_code);
        return;
    }

    thread_t* thread = current_thread;
    current_thread = nullptr;

    const LONG prev = InterlockedExchange(&thread->state, THREAD_STATE_FINISHED);
    if (prev == THREAD_STATE_DETACHED)
        allocator_free(thread, thread->allocator);

    _endthreadex(exit_code);
}

void terminate_thread(thread_t* const thread, const u32_t exit_code) {
    TerminateThread(thread->handle, exit_code);
    CloseHandle(thread->handle);
    allocator_free(thread, thread->allocator);
}

b8_t join_thread(thread_t* const thread, void** const result) {
    if (WaitForSingleObject(thread->handle, INFINITE) != WAIT_OBJECT_0) return false;

    CloseHandle(thread->handle);
    thread->handle = INVALID_HANDLE_VALUE;

    if (result != nullptr) *result = thread->retval;

    const LONG st = InterlockedCompareExchange(&thread->state, THREAD_STATE_FINISHED, THREAD_STATE_FINISHED);
    if (st != THREAD_STATE_DETACHED)
        allocator_free(thread, thread->allocator);

    return true;
}

b8_t detach_thread(thread_t* const thread) {
    const LONG prev = InterlockedCompareExchange(&thread->state, THREAD_STATE_DETACHED, THREAD_STATE_JOINABLE);
    if (prev == THREAD_STATE_JOINABLE) {
        CloseHandle(thread->handle);
        thread->handle = INVALID_HANDLE_VALUE;
        return true;
    }
    if (prev == THREAD_STATE_FINISHED) {
        CloseHandle(thread->handle);
        allocator_free(thread, thread->allocator);
        return true;
    }

    return false;
}

thread_t* get_current_thread(void) {
    return current_thread;
}

u32_t get_thread_id(const thread_t* const thread) {
    return thread->thread_id;
}
