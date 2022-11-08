#pragma once
#include "spinlock.h"

// *-------------* PROJECT 03 *-------------*
typedef struct xem_t {
  struct spinlock lock;
  uint sema;                  // semaphore varible
  int waitnum;                // the number of waiting threads
  struct proc* list[64];      // waiting list (queue)
  int front;
  int rear;
}xem_t;

typedef struct rwlock_t {
  struct xem_t lock;          // binary semaphore
  struct xem_t writelock;     // used to allow ONE writer or MANY readers
  int readers;                // count of readers reading in critical section
}rwlock_t;
// ******************************************

// *-------------* PROJECT 04 *-------------*
typedef struct safe_guard {
  xem_t lock;
  xem_t writelock;
  int readers;
  int fd;
}thread_safe_guard;
// ******************************************