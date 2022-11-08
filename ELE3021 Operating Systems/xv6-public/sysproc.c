#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getppid(void)
{
	return getppid();
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  //addr = myproc()->sz;
  addr = growproc(n);
  //if(growproc(n) < 0)
  if(addr < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// yield CPU
int sys_yield(void)
{
  yield();
  return 0;
}

// return level of MLFQ
int sys_getlev(void)
{
  return myproc()->mlfq.level;
}

// inquire to obtain CPU share(%)
int sys_set_cpu_share(void)
{
  int share;
  if (argint(0, &share) < 0)
    return -1;

  return set_cpu_share(share);
}

// thread create
int sys_thread_create(void)
{
	int thread, routine, arg;

  if((argint(0, &thread) < 0) || (argint(1, &routine) < 0) || (argint(2, &arg) < 0))
    return -1;

  return thread_create((thread_t*)thread, (void*)routine, (void*)arg);
}

// thread exit
int sys_thread_exit(void)
{
  int retval;

  if(argint(0, &retval) < 0)
    return -1;
    
  thread_exit((void*)retval);
  return 0;
}

// thread join
int sys_thread_join(void)
{
  int thread, retval;

  if((argint(0, &thread) < 0) || (argint(1, &retval) < 0))
    return -1;
  return thread_join((thread_t)thread, (void**)retval);
}

// semaphore init
int sys_xem_init(void)
{
  int sem;

  if(argint(0, &sem) < 0)
    return -1;
  
  return xem_init((xem_t*)sem);
}

// semaphore wait
int sys_xem_wait(void)
{
  int sem;

  if(argint(0, &sem) < 0)
    return -1;
  
  return xem_wait((xem_t*)sem);
}

// semaphore signal
int sys_xem_unlock(void)
{
  int sem;

  if(argint(0, &sem) < 0)
    return -1;
  
  return xem_unlock((xem_t*)sem);
}

int sys_rwlock_init(void)
{
  int rw;

  if(argint(0, &rw) < 0)
    return -1;
  
  return rwlock_init((rwlock_t*)rw);
}

int sys_rwlock_acquire_readlock(void)
{
  int rw;

  if(argint(0, &rw) < 0)
    return -1;
  
  return rwlock_acquire_readlock((rwlock_t*)rw);
}

int sys_rwlock_acquire_writelock(void)
{
  int rw;

  if(argint(0, &rw) < 0)
    return -1;
  
  return rwlock_acquire_writelock((rwlock_t*)rw);
}

int sys_rwlock_release_readlock(void)
{
  int rw;

  if(argint(0, &rw) < 0)
    return -1;
  
  return rwlock_release_readlock((rwlock_t*)rw);
}

int sys_rwlock_release_writelock(void)
{
  int rw;

  if(argint(0, &rw) < 0)
    return -1;
  
  return rwlock_release_writelock((rwlock_t*)rw);
}