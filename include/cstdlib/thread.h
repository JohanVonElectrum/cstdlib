#pragma once

#include <cstdlib/alloc.h>
#include <cstdlib/nullability.h>

/**
 * Opaque struct representing a thread.
 * The actual implementation is platform-specific and should not be accessed directly.
 */
typedef struct thread_t thread_t;

/**
 * Function pointer type for thread functions.
 * The function should take a single `void*` parameter and return a `void*` result.
 */
typedef void*(*thread_fn_t)(void*);

/**
 * Generate `thread_t` for the current thread.
 * Typically, this should only be called once from the main thread at the start of the program to set up the main thread's `thread_t`.
 *
 * @pre must be called from a thread that has not already been set up with `setup_thread` or spawned with `spawn_thread`.
 *
 * @param allocator [in, non-null] allocator to use for allocating the `thread_t` struct.
 * @retval nullptr error allocating `thread_t`.
 * @retval non-null successfully generated `thread_t`.
 *
 * @see thread_t
 * @see spawn_thread
 */
CSTDLIB_EXPORT RET_NULLABLE thread_t* setup_thread(IN_NONNULL const allocator_t* allocator) NONNULL_ARGS();

/**
 * Spawn a new thread that executes the given function with the given parameter.
 *
 * @param fn [in, non-null] function to execute in the new thread.
 * @param param [in, nullable] parameter to pass to the thread function.
 * @param allocator [in, non-null] allocator to use for allocating the `thread_t` struct.
 * @retval nullptr error allocating `thread_t` or creating the thread.
 * @retval non-null successfully created thread.
 *
 * @see thread_t
 */
CSTDLIB_EXPORT RET_NULLABLE thread_t* spawn_thread(IN_NONNULL thread_fn_t fn, IN_NULLABLE void* param,
                                                   IN_NONNULL const allocator_t* allocator) NONNULL_ARGS(1, 3);

/**
 * Exit the current thread with the given exit code.
 *
 * @param exit_code exit code to return from the thread.
 *
 * @see terminate_thread
 */
CSTDLIB_EXPORT void exit_thread(u32_t exit_code);

/**
 * Terminate the given thread with the given exit code.
 *
 * @param thread [in, non-null] thread to terminate.
 * @param exit_code exit code to return from the thread.
 *
 * @see thread_t
 * @see exit_thread
 */
CSTDLIB_EXPORT void terminate_thread(INOUT_NONNULL thread_t* thread, u32_t exit_code) NONNULL_ARGS();

/**
 * Wait for the given thread to finish.
 *
 * @pre thread is joinable (aka not detached or already joined).
 *
 * @param thread [in, non-null] thread to join.
 * @param result [out, nullable] pointer to store the thread's return value.
 * @retval false if an error occurred while joining the thread.
 * @retval true successfully joined the thread and stored its return value in `result` (if `result` is non-null).
 *         If `result` is null, the thread's return value will be discarded.
 *
 * @see thread_t
 * @see detach_thread
 */
CSTDLIB_EXPORT b8_t join_thread(INOUT_NONNULL thread_t* thread, OUT_NULLABLE void** result) NONNULL_ARGS(1);

/**
 * Detach the given thread, allowing it to run independently and freeing its resources when it finishes.
 *
 * @pre thread is joinable (aka not detached or already joined) or finished.
 *
 * @param thread [in, non-null] thread to detach.
 * @retval false if an error occurred while detaching the thread.
 * @retval true successfully detached the thread. If the thread has already finished, its resources will be freed immediately.
 *
 * @see thread_t
 * @see join_thread
 */
CSTDLIB_EXPORT b8_t detach_thread(INOUT_NONNULL thread_t* thread) NONNULL_ARGS();

/**
 * Get the `thread_t` for the current thread.
 *
 * @pre must be called from a thread that has been set up with `setup_thread` or spawned with `spawn_thread`.
 *
 * @retval nullptr if called from a thread that has not been set up with `setup_thread` or spawned with `spawn_thread`.
 * @retval non-null `thread_t` for the current thread.
 *
 * @see thread_t
 */
CSTDLIB_EXPORT RET_NULLABLE thread_t* get_current_thread(void);

/**
 * Get the unique ID of the given thread.
 *
 * @param thread [in, non-null] thread to get the ID of.
 * @return unique ID of the thread.
 *
 * @see thread_t
 */
CSTDLIB_EXPORT u32_t get_thread_id(IN_NONNULL const thread_t* thread) NONNULL_ARGS();
