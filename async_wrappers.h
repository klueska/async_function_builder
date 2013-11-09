/* See COPYING.LESSER for copyright information. */
/* Kevin Klues <klueska@cs.berkeley.edu>	*/
/* Andrew Waterman <waterman@cs.berkeley.edu>	*/

#ifndef __ASYNC_WRAPPERS__
#define __ASYNC_WRAPPERS__

#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/* BEGIN GLIBC-specific code */
#ifdef __GLIBC__
int __open(const char*, int, ...);
int __close(int);
ssize_t __read(int, void*, size_t);
ssize_t __write(int, const void*, size_t);
#endif

#ifdef __IN_LIBC_WRAPPERS_C
# define __wrap_open __open
# define __wrap_close __close
# define __wrap_read __read
# define __wrap_write __write
#else
# ifdef __GLIBC__
#  define open __open
#  define close __close
#  define read __read
#  define write __write
# endif
#endif
/* END GLIBC-specific code */

// Need to fix this to yield uthread / bring it back up when syscall done
#define __async_simulate_blocking(__func, ...) \
({ \
  typeof(__func(__VA_ARGS__)) result; \
  void *handle = async_start(__func, __VA_ARGS__); \
  async_wait(handle, &result); \
  (typeof(__func(__VA_ARGS__)))result; \
})

#define __async_simulate_blocking_noreturn(__func, ...) \
({ \
  void *handle = async_start(__func, __VA_ARGS__); \
  (void)async_wait(handle, NULL); \
})

#define async_start(__func, ...) \
({ \
  typeof(__func(__VA_ARGS__)) do_##__func(void *arg) { \
    return __func(__VA_ARGS__); \
  } \
  __async_start(&do_##__func, false); \
})

#define async_start_detached(__func, callback, ...) \
({ \
  typeof(__func(__VA_ARGS__)) do_##__func(void *arg) { \
    typeof(__func(__VA_ARGS__)) temp = __func(__VA_ARGS__); \
    callback(temp); \
    return temp; \
  } \
  (void)__async_start(&do_##__func, true); \
})

#define async_wait(handle, result) \
  __async_wait(handle, ((void**)result));
 
void *__async_start(void *func, bool detached);
int __async_wait(void *handle, void **result);

#endif // __ASYNC_WRAPPERS__

