#include "async_wrappers.h"
#include <pthread.h>
#include <stdlib.h>

extern inline void *__async_start(void *func, bool detached) {
  pthread_attr_t attr;
  void *handle = malloc(sizeof(pthread_t));
  pthread_attr_init(&attr);
  if(detached)
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  pthread_create((pthread_t*)handle, &attr, func, NULL);
  return handle;
}

extern inline int __async_wait(void *handle, void **result) {
  int ret = pthread_join(*((pthread_t*)handle), result);
  free(handle);
  return ret;
}
