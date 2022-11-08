# Implement the pread, pwrite System Call


## file.c

### pread()

``` c
int
pread(struct file *f, char *addr, int n, int offset)
{
  int r;

  if(f->readable == 0)
    return -1;
  if(f->type == FD_PIPE)
    return piperead(f->pipe, addr, n);
  if(f->type == FD_INODE){
    ilock(f->ip);
    if((r = readi(f->ip, addr, offset, n)) > 0)
      offset += r;
    iunlock(f->ip);
    return r;
  }
  panic("pread");
}
```

It is exactly the same as `fileread()` except that `offset` is added as the parameter of the function and `offset` is used instead of `f->off`.



### pwrite()

``` c
int
pwrite(struct file *f, char *addr, int n, int offset)
{
  int r;

  if(f->writable == 0)
    return -1;
  if(f->type == FD_PIPE)
    return pipewrite(f->pipe, addr, n);
  if(f->type == FD_INODE){
    // write a few blocks at a time to avoid exceeding
    // the maximum log transaction size, including
    // i-node, indirect block, allocation blocks,
    // and 2 blocks of slop for non-aligned writes.
    // this really belongs lower down, since writei()
    // might be writing a device like the console.
    int max = ((MAXOPBLOCKS-1-1-2) / 2) * 512;
    int i = 0;
    while(i < n){
      int n1 = n - i;
      if(n1 > max)
        n1 = max;

      begin_op();
      ilock(f->ip);
      if ((r = writei(f->ip, addr + i, offset, n1)) > 0)
        offset += r;
      iunlock(f->ip);
      end_op();

      if(r < 0)
        break;
      if(r != n1)
        panic("short filewrite");
      i += r;
    }
    return i == n ? n : -1;
  }
  panic("pwrite");
}
```

It is exactly the same as `filewrite()` except that `offset` is added as the parameter of the function and `offset` is used instead of `f->off`.



## fs.c

``` c
int
writei(struct inode *ip, char *src, uint off, uint n)
{
  uint tot, m;
  struct buf *bp;

  if(ip->type == T_DEV){
    if(ip->major < 0 || ip->major >= NDEV || !devsw[ip->major].write)
      return -1;
    return devsw[ip->major].write(ip, src, n);
  }

  //if(off > ip->size || off + n < off)
  if(off + n < off)
    return -1;
  if(off + n > MAXFILE*BSIZE)
    return -1;

  // *-------------* PROJECT 04 *-------------*
  if(off > ip->size)
  {
    ip->size = off + n;
    iupdate(ip);
  }
  // ******************************************

  for(tot=0; tot<n; tot+=m, off+=m, src+=m){
    bp = bread(ip->dev, bmap(ip, off/BSIZE));
    m = min(n - tot, BSIZE - off%BSIZE);
    memmove(bp->data + off%BSIZE, src, m);
    log_write(bp);
    brelse(bp);
  }

  if(n > 0 && off > ip->size){
    ip->size = off;
    iupdate(ip);
  }
  return n;
}
```

`writei()` was modified to handle the case where `off` is greater than `ip->size`.





## sysfile.c

``` c
int
sys_pwrite(void)
{
  char *addr;
  int n, offset;
  struct file *f;

  if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argint(3, &offset) < 0 || argptr(1, &addr, n) < 0)
    return -1;
  return pwrite(f, addr, n, offset);
}

int
sys_pread(void)
{
  char *addr;
  int n, offset;
  struct file *f;

  if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argint(3, &offset) < 0 || argptr(1, &addr, n) < 0)
    return -1;
  return pread(f, addr, n, offset);
}
```

New system calls are implemented in `sysfile.c`.



## defs.h

``` c
int             pread(struct file *f, char *addr, int n, int offset);
int             pwrite(struct file *f, char *addr, int n, int offset);
```

Function definitions are registered in `defs.h` to use `pread()`, `pwrite()`.



## syscall.h

``` c
#define SYS_pread  38
#define SYS_pwrite  39
```

New system calls are defined in `syscall.h`.



## syscall.c

``` c
extern int sys_pread(void);
extern int sys_pwrite(void);

[SYS_pread] sys_pread,
[SYS_pwrite] sys_pwrite,
```

New system calls are registered in `syscall.c`.



## user.h

``` c
int pread(int, void*, int, int);
int pwrite(int, void*, int, int);
```

New functions are defined in `user.h`.



## usys.S

``` c
SYSCALL(pread)
SYSCALL(pwrite)
```

New system calls are registered in `usys.S`.



# Implement a Thread-Safe Read and Write User Library

## semaphore.h

``` c
typedef struct safe_guard {
  xem_t lock;
  xem_t writelock;
  int readers;
  int fd;
}thread_safe_guard;
```

A structure `safe_guard` is defined to be used for thread-safe read and write.

- `lock` is used to protect variables inside `readers`.

- `writelock` is used to guarantee mutual exclusion.

- `readers` indicates how many readers exist in the critical section.

- `fd` is file descriptor received as a parameter of `thread_safe_guard_init()`



## types.h

``` c
typedef struct safe_guard thread_safe_guard;
```

New content was added as specified in the assignment description.



## tsafe.c

A new file was created to implement a library for thread-safe read and write.

``` c
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
```

`thread_safe_guard_init()` creates a new `thread_safe_guard` structure through `malloc()`, initializes the variables inside the structure, and then returns it.

At this time, put `fd` passed as a parameter of the function into `file_guard->fd`.

It also initializes the `reader` variable to 0 and initializes the lock.



``` c
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
```

`thread_safe_pread()` behaves like a combination of `rwlock_acquire_readlock()` and `rwlock_release_readlock()` of reader-writer locks.

First, if `lock` is acquired through `xem_wait()`, `readers` is incremented by 1. And as in reader-writer lock, if it is the first reader, `writelock` is acquired through `xem_wait()`.

After completing the above process, it can be read safely, so execute `pread()` using the values passed as arguments.

When `pread()` is finished, `readers` is automatically decremented by 1, and it checks if it is the last reader. If it is the last reader, release the holding `writelock` and release `lock`, otherwise only release `lock` because another reader needs `writelock` for `pread()`.

When all execution is finished, the `size` obtained from `pread()` is returned.



``` c
int thread_safe_pwrite(thread_safe_guard* file_guard, void* addr, int n, int off)
{
  int size;
  xem_wait(&file_guard->writelock);
  size = pwrite(file_guard->fd, addr, n, off);
  xem_unlock(&file_guard->writelock);

  return size;
}
```

`thread_safe_pwrite()` behaves like a combination of `rwlock_acquire_writelock()` and `rwlock_release_writelock()` of reader-writer locks.

If `writelock` is acquired through `xem_wait()`, `pwrite()` is executed safely, and `writelock` is released through `xem_unlock()`.

When all execution is complete, `size` is returned.



``` c
void thread_safe_guard_destroy(thread_safe_guard * file_guard)
{
  free((void*)file_guard);
  return;
}
```

In `thread_safe_guard_destroy()`, the space allocated by `malloc()` in `thread_safe_guard_init()` is freed through `free()`.




## user.h

``` c
thread_safe_guard* thread_safe_guard_init(int fd);
int thread_safe_pread(thread_safe_guard* file_guard, void* addr, int n, int off);
int thread_safe_pwrite(thread_safe_guard* file_guard, void* addr, int n, int off);
void thread_safe_guard_destroy(thread_safe_guard * file_guard);
```

New functions are defined in `user.h`.



## Makefile

``` c
EXTRA=\
	mkfs.c ulib.c user.h cat.c echo.c forktest.c grep.c kill.c\
	ln.c ls.c mkdir.c rm.c stressfs.c usertests.c wc.c zombie.c\
	printf.c umalloc.c tsafe.c ...
...
ULIB = ulib.o usys.o printf.o umalloc.o tsafe.o
```

I modified the contents of the variable that defines the files necessary to build the user library in the Makefile.
