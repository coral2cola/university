// *-------------* PROJECT 04 *-------------*

#include "types.h"
#include "user.h"

thread_safe_guard* thread_safe_guard_init(int fd)
{
  thread_safe_guard* file_guard = (thread_safe_guard*)malloc(sizeof(thread_safe_guard));
  memset(file_guard, 0, sizeof(thread_safe_guard));
  file_guard->readers = 0;
  file_guard->fd = fd;
  xem_init(&file_guard->lock);
  xem_init(&file_guard->writelock);

  return file_guard;
}

int thread_safe_pread(thread_safe_guard* file_guard, void* addr, int n, int off)
{
  int size;

  xem_wait(&file_guard->lock);
  file_guard->readers++;
  if(file_guard->readers == 1)
    xem_wait(&file_guard->writelock);
  
  size = pread(file_guard->fd, addr, n, off);

  file_guard->readers--;
  if(file_guard->readers == 0)
    xem_unlock(&file_guard->writelock);

  xem_unlock(&file_guard->lock);
  
  return size;
}

int thread_safe_pwrite(thread_safe_guard* file_guard, void* addr, int n, int off)
{
  int size;
  xem_wait(&file_guard->writelock);
  size = pwrite(file_guard->fd, addr, n, off);
  xem_unlock(&file_guard->writelock);

  return size;
}

void thread_safe_guard_destroy(thread_safe_guard * file_guard)
{
  free((void*)file_guard);
  return;
}