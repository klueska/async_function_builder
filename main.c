/* See COPYING.LESSER for copyright information. */
/* Kevin Klues <klueska@cs.berkeley.edu>	*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
#include "async_wrappers.h"

char buf[1000];
volatile int detached_done = false;

void read_done(ssize_t res) {
  printf("Deferred read: res %d, str \"%s\"\n", (int)res, buf);
  detached_done = true;
}

int main (int argc, char **argv)
{
  void *ptr;
  void *handle = async_start(malloc, 1000);
  printf("Async malloc started; about to wait...\n");
  async_wait(handle, &ptr);
  printf("Async malloc, result: %p\n", ptr);
  
  int fd = open("./temp", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  printf("Blocking open, fd: %d\n", fd);

  char str[] = "test string";
  ssize_t write_res = write(fd, str, sizeof(str));
  assert(write_res == sizeof(str));

  int close_res;
  handle = async_start(close, fd);
  printf("Async close started; about to wait...\n");
  async_wait(handle, &close_res);
  printf("Async close, result: %d\n", close_res);

  fd = open("./temp", O_RDWR, S_IRUSR | S_IWUSR);
  printf("Blocking open, fd: %d\n", fd);

  async_start_detached(read, read_done, fd, buf, sizeof(buf)-1);
  while(!detached_done);

  int unlink_res = unlink("./temp");
  printf("Blocking unlink, result: %d\n", unlink_res);

  printf("Program finishing\n");

  return 0;
}
