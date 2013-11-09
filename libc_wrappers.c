#include <stdarg.h>

#define __IN_LIBC_WRAPPERS_C
#include "async_wrappers.h"

#ifdef __GLIBC__

int open(const char* path, int oflag, ...)
{
  va_list vl;
  va_start(vl, oflag);
  int mode = va_arg(vl, int);
  va_end(vl);

  return __async_simulate_blocking(__wrap_open, path, oflag, mode);
}

int close(int fd)
{
  return __async_simulate_blocking(__wrap_close, fd);
}

ssize_t read(int fd, void* buf, size_t sz)
{
  return __async_simulate_blocking(__wrap_read, fd, buf, sz);
}

ssize_t write(int fd, const void* buf, size_t sz)
{
  return __async_simulate_blocking(__wrap_write, fd, buf, sz);
}

#endif
