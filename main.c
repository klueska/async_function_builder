/* See COPYING.LESSER for copyright information. */
/* Kevin Klues <klueska@cs.berkeley.edu>	*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include "async_wrappers.h"

volatile int detached_done = false;

void malloc_done(void *ptr) {
  printf("Deferred async malloc ptr: %p\n", ptr);
  detached_done = true;
}

int main (int argc, char **argv)
{
  void *ptr1 = malloc(1000);
  printf("Blocking malloc ptr1: %p\n", ptr1);

  void *ptr2;
  void *handle = async_malloc(1000);
  printf("Async malloc not done for ptr2: %p\n", ptr2);
  async_wait(handle, &ptr2);
  printf("Async malloc ptr2: %p\n", ptr2);
  
  int val1= open("./temp", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  printf("Blocking open val1: %d\n", val1);

  int val2;
  handle = async_close(val1);
  printf("Async close not done for val2: %d\n", val2);
  async_wait(handle, &val2);
  printf("Async close val2: %d\n", val2);

  int val3 = unlink("./temp");
  printf("Blocking unlink val3: %d\n", val3);

  async_malloc_detached(malloc_done, 1000);
  while(!detached_done);
  printf("Program finishing\n");

  return 0;
}
