#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
//#include "spinlock.h"

struct
{
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

static struct proc *initproc;

int nextpid = 1;
extern void forkret(void);
extern void trapret(void);

static void wakeup1(void *chan);

struct spinlock ticklock;

// // *-------------* PROJECT 03 *-------------*
// struct xem_t {
//   struct spinlock lock;
//   uint sema;                  // semaphore varible
//   int waitnum;                // the number of waiting threads
//   struct proc* list[NPROC]    // waiting list (queue)
//   int front;
//   int rear;
// };

// struct rwlock_t {
//   struct xem_t lock;          // binary semaphore
//   struct xem_t writelock;     // used to allow ONE writer or MANY readers
//   int readers;                // count of readers reading in critical section
// };
// // ******************************************

void pinit(void)
{
  initlock(&ptable.lock, "ptable");
}

// Must be called with interrupts disabled
int cpuid()
{
  return mycpu() - cpus;
}

// Must be called with interrupts disabled to avoid the caller being
// rescheduled between reading lapicid and running through the loop.
struct cpu *
mycpu(void)
{
  int apicid, i;

  if (readeflags() & FL_IF)
    panic("mycpu called with interrupts enabled\n");

  apicid = lapicid();
  // APIC IDs are not guaranteed to be contiguous. Maybe we should have
  // a reverse map, or reserve a register to store &cpus[i].
  for (i = 0; i < ncpu; ++i)
  {
    if (cpus[i].apicid == apicid)
      return &cpus[i];
  }
  panic("unknown apicid\n");
}

// Disable interrupts so that we are not rescheduled
// while reading proc from the cpu structure
struct proc *
myproc(void)
{
  struct cpu *c;
  struct proc *p;
  pushcli();
  c = mycpu();
  p = c->proc;
  popcli();
  return p;
}

//PAGEBREAK: 32
// Look in the process table for an UNUSED proc.
// If found, change state to EMBRYO and initialize
// state required to run in the kernel.
// Otherwise return 0.
static struct proc *
allocproc(void)
{
  struct proc *p;
  char *sp;

  acquire(&ptable.lock);

  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if (p->state == UNUSED)
      goto found;

  release(&ptable.lock);
  return 0;

found:
  p->state = EMBRYO;
  p->pid = nextpid++;

  release(&ptable.lock);

  // Allocate kernel stack.
  if ((p->kstack = kalloc()) == 0)
  {
    p->state = UNUSED;
    return 0;
  }
  sp = p->kstack + KSTACKSIZE;

  // Leave room for trap frame.
  sp -= sizeof *p->tf;
  p->tf = (struct trapframe *)sp;

  // Set up new context to start executing at forkret,
  // which returns to trapret.
  sp -= 4;
  *(uint *)sp = (uint)trapret;

  sp -= sizeof *p->context;
  p->context = (struct context *)sp;
  memset(p->context, 0, sizeof *p->context);
  p->context->eip = (uint)forkret;

  // *-------------* PROJECT 01 *-------------*
  p->mode = MLFQ;
  p->yield = 0;

  memset(&p->stride, 0, sizeof p->stride);
  memset(&p->mlfq, 0, sizeof p->mlfq);
  // ******************************************

  // *-------------* PROJECT 02 *-------------*
  p->tid = 0;
  //p->tcnt = 0;
  p->masterproc = 0;
  p->retval = 0;
  //p->stackbase = 0;

  memset(&p->freelist, 0, sizeof p->freelist);
  // ******************************************

  return p;
}

//PAGEBREAK: 32
// Set up first user process.
void userinit(void)
{
  struct proc *p;
  extern char _binary_initcode_start[], _binary_initcode_size[];

  p = allocproc();

  initproc = p;
  if ((p->pgdir = setupkvm()) == 0)
    panic("userinit: out of memory?");
  inituvm(p->pgdir, _binary_initcode_start, (int)_binary_initcode_size);
  p->sz = PGSIZE;
  memset(p->tf, 0, sizeof(*p->tf));
  p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
  p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
  p->tf->es = p->tf->ds;
  p->tf->ss = p->tf->ds;
  p->tf->eflags = FL_IF;
  p->tf->esp = PGSIZE;
  p->tf->eip = 0; // beginning of initcode.S

  safestrcpy(p->name, "initcode", sizeof(p->name));
  p->cwd = namei("/");

  // this assignment to p->state lets other cores
  // run this process. the acquire forces the abosve
  // writes to be visible, and the lock is also needed
  // because the assignment might not be atomic.
  acquire(&ptable.lock);

  p->state = RUNNABLE;

  release(&ptable.lock);
}

// Grow current process's memory by n bytes.
// Return 0 on success, -1 on failure.
int growproc(int n)
{
  uint sz, old;
  struct proc *curproc = myproc();
  struct proc *p;

  // *-------------* PROJECT 02 *-------------*
  if (curproc->masterproc == 0)
    p = curproc;
  else
    p = curproc->masterproc;

  sz = p->sz;
  old = sz;

  acquire(&ptable.lock);

  if (n > 0)
  {
    if ((sz = allocuvm(curproc->pgdir, sz, sz + n)) == 0)
    {
      release(&ptable.lock);
      return -1;
    }
  }
  else if (n < 0)
  {
    if ((sz = deallocuvm(curproc->pgdir, sz, sz + n)) == 0)
    {  
      release(&ptable.lock);
      return -1;
    }
  }
  p->sz = sz;
  release(&ptable.lock);

  switchuvm(curproc);
  return old;

  // ******************************************
}

// Create a new process copying p as the parent.
// Sets up stack to return as if from system call.
// Caller must set state of returned proc to RUNNABLE.
int fork(void)
{
  // cprintf("fork()\n");
  int i, pid;
  struct proc *np;
  struct proc *curproc = myproc();

  // Allocate process.
  if ((np = allocproc()) == 0)
  {
    return -1;
  }

  // *-------------* PROJECT 02 *-------------*
  if (curproc->tid == 0) // process
  {
    np->pgdir = copyuvm(curproc->pgdir, curproc->sz);
    // cprintf("finish-1\n");
  }
  else if (curproc->tid > 0) // thread
  {
    np->pgdir = copyuvm(curproc->pgdir, curproc->masterproc->sz);
    // cprintf("finish-2\n");
  }
  else // error
  {
    cprintf("error in fork\n");
    return -1;
  }
  // ******************************************

  // Copy process state from proc.
  if (np->pgdir == 0)
  {
    kfree(np->kstack);
    np->kstack = 0;
    np->state = UNUSED;
    return -1;
  }
  np->sz = curproc->sz;
  np->parent = curproc;
  *np->tf = *curproc->tf;

  // Clear %eax so that fork returns 0 in the child.
  np->tf->eax = 0;

  for (i = 0; i < NOFILE; i++)
    if (curproc->ofile[i])
      np->ofile[i] = filedup(curproc->ofile[i]);
  np->cwd = idup(curproc->cwd);

  safestrcpy(np->name, curproc->name, sizeof(curproc->name));

  pid = np->pid;

  acquire(&ptable.lock);

  np->state = RUNNABLE;

  release(&ptable.lock);

  return pid;
}

// *-------------* PROJECT 02 *-------------*
void return_resource(struct proc* p)
{
  // similar to the part of wait()
  kfree(p->kstack);
  p->kstack = 0;

  p->pid = 0;
  p->parent = 0;
  p->name[0] = 0;
  p->killed = 0;
  p->state = UNUSED;
  p->tid = 0;

  p->masterproc->freelist.freespace[p->masterproc->freelist.num] = p->stackbase;
  p->masterproc->freelist.num += 1;
  // cprintf("== NUM : %d ==\n", p->masterproc->freelist.num);
  p->masterproc->tcnt--;
  p->masterproc = 0;

  deallocuvm(p->pgdir, p->sz, p->stackbase);
  // p->stackbase = 0;
}
// ******************************************

// Exit the current process.  Does not return.
// An exited process remains in the zombie state
// until its parent calls wait() to find out it exited.
void exit(void)
{
  // cprintf("exit()\n");
  struct proc *curproc = myproc();
  struct proc *p;
  int fd;

  if (curproc == initproc)
    panic("init exiting");

  // *-------------* PROJECT 02 *-------------*
  if (curproc->tid == 0)
  {
    acquire(&ptable.lock);
    for(;;)
    {
      for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
      {
        if(p->masterproc == curproc)
        {
          if(p->pid != curproc->pid)
          {
            cprintf("exit error\n");
            return;
          }

          if(p->state == ZOMBIE)
          {
            return_resource(p);
            // cprintf("%d\n", curproc->tcnt);
          }
          else
          {
            p->killed = 1;
            // cprintf("WAKEUP 1\n");
            wakeup1(p);
          }
        }
      }
      if(curproc->tcnt == 0)
      {
        // cprintf("** BREAK **\n");
        release(&ptable.lock);
        break;
      }

      sleep(curproc, &ptable.lock);
    }
  }
  // ******************************************
  
  // Close all open files.
  for (fd = 0; fd < NOFILE; fd++)
  {
    if (curproc->ofile[fd])
    {
      fileclose(curproc->ofile[fd]);
      curproc->ofile[fd] = 0;
    }
  }

  begin_op();
  iput(curproc->cwd);
  end_op();
  curproc->cwd = 0;

  acquire(&ptable.lock);

  // *-------------* PROJECT 02 *-------------*
  if (curproc->tid == 0)
  {
    // cprintf("WAKEUP 2\n");
    wakeup1(curproc->parent);
    mlfqinfo.usingticket -= curproc->stride.ticket;
  }
  else if (curproc->tid > 0)
  {
    if (curproc->masterproc != 0)
    {
      // cprintf("WAKEUP 3\n");
      wakeup1(curproc->masterproc);
      curproc->masterproc->killed = 1;
    }
  }
  else
  {
    cprintf("exit error\n");
    return;
  }

  // ******************************************

  // Pass abandoned children to init.
  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->parent == curproc)
    {
      p->parent = initproc;
      if (p->state == ZOMBIE)
      {
        // cprintf("WAKEUP 4\n");
        wakeup1(initproc);
      }
    }
  }

  // Jump into the scheduler, never to return.
  curproc->state = ZOMBIE;
  sched();
  panic("zombie exit");
}


// Wait for a child process to exit and return its pid.
// Return -1 if this process has no children.
int wait(void)
{
  struct proc *p;
  int havekids, pid;
  struct proc *curproc = myproc();

  acquire(&ptable.lock);
  for (;;)
  {
    // Scan through table looking for exited children.
    havekids = 0;
    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    {
      if (p->parent != curproc)
        continue;
      havekids = 1;
      if (p->state == ZOMBIE)
      {
        // Found one.
        pid = p->pid;
        kfree(p->kstack);
        p->kstack = 0;
        freevm(p->pgdir);
        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        p->state = UNUSED;

        release(&ptable.lock);
        return pid;
      }
    }

    // No point waiting if we don't have any children.
    if (!havekids || curproc->killed)
    {
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(curproc, &ptable.lock); //DOC: wait-sleep
  }
}

// add ticks
// and check priority boost frequency, time allotment, time quantum
int tick_adder(void)
{
  // add ticks
  // MLFQ ticks (for priority boosting)
  // proc ticks (for RR and MLFQ)

  acquire(&ptable.lock);

  acquire(&ticklock);
  myproc()->mlfq.qticks++;
  mlfqinfo.MLFQticks++;
  release(&ticklock);

  struct proc *p;

  // boost priority (frequency : 100 ticks)
  if (mlfqinfo.MLFQticks >= 100)
  {
    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    {
      if (p->state != RUNNABLE)
        continue;

      p->mlfq.level = 0; // highest priority queue
      p->mlfq.prior = 0;
      p->mlfq.qticks = 0;
    }
    mlfqinfo.hpriority = 0;
    mlfqinfo.MLFQticks = 0;
  }

  p = myproc();
  
  int lev = p->mlfq.level;
  int ticks = p->mlfq.qticks;

  // 1. check time allotment
  // 2. check time quantum
  if (lev == 0)
  {
    if (ticks >= 5)
    {
      p->mlfq.level += 1; // move to lower priority queue
      p->mlfq.qticks = 0;
    }
    else if (ticks % 1 == 0)
    {
      p->yield = 1; // yield process
      mlfqinfo.hpriority += 1;
      p->mlfq.prior = mlfqinfo.hpriority;
    }
  }
  else if (lev == 1)
  {
    if (ticks >= 10)
    {
      p->mlfq.level += 1; // move to lower priority queue
      p->mlfq.qticks = 0;
    }
    else if (ticks % 2 == 0)
    {
      p->yield = 1; // yield process
      mlfqinfo.hpriority += 1;
      p->mlfq.prior = mlfqinfo.hpriority;
    }
  }
  else if (lev == 2)
  {
    if (ticks >= 4)
    {
      p->yield = 1; // yield process
      mlfqinfo.hpriority += 1;
      p->mlfq.prior = mlfqinfo.hpriority;
    }
  }
  else
  {
    return -1;
  }

  release(&ptable.lock);
  return 0;
}

// MLFQ scheduler
void scheduler2(void)
{
  struct proc *p;
  struct cpu *c = mycpu();
  c->proc = 0;

  int num = 2; // MLFQ level
  int priormin = 10000;

  struct proc *next = 0;
  int check = 0;

  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->state != RUNNABLE || p->mode != MLFQ)
      continue;

    if (p->mlfq.level < num)
    {
      next = p;
      check = 1;
    }
    else if (p->mlfq.level == num)
    {
      if (p->mlfq.prior < priormin)
      {
        next = p;
        check = 1;
      }
    }

    if (check == 1)
    {
      num = p->mlfq.level;
      priormin = p->mlfq.prior;
      check = 0;
    }
  }

  if ((p = next))
  {
    c->proc = next;
    switchuvm(next);
    next->state = RUNNING;
    next->yield = 0;

    swtch(&(c->scheduler), next->context);
    switchkvm();

    c->proc = 0;
  }

  // add stride
  mlfqinfo.qstride.pass += (100l / (float)(100 - mlfqinfo.usingticket));
}

// get current stride
float get_stride(struct proc *proc)
{
  struct proc *p;
  int num = 0;

  if (proc->mode != STRIDE)
    panic("get stride");

  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->pid == proc->pid) /* project 2 */
      num++;
  }

  return 100l / (float)proc->stride.ticket / num;
}

//PAGEBREAK: 42
// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run
//  - swtch to start running that process
//  - eventually that process transfers control
//      via swtch back to the scheduler.
void scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu();
  c->proc = 0;

  struct proc *next;

  for (;;)
  {
    // Enable interrupts on this processor.
    sti();

    // Loop over process table looking for process to run.
    acquire(&ptable.lock);

    next = 0;

    float mlfqpass = mlfqinfo.qstride.pass;
    int num = 0;

    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    {
      if (p->state != RUNNABLE)
        continue;

      if (p->mode == STRIDE && p->stride.pass < mlfqpass)
      {
        mlfqpass = p->stride.pass;
        next = p;
      }
      num += 1;
    }

    // next proc is in MLFQ
    if (next == 0)
    {
      scheduler2();

      if (num == 0)
      {
        mlfqinfo.qstride.pass = 0;
      }
    }
    else // next proc is in stride scheduler
    {
      c->proc = next;
      switchuvm(next);
      next->state = RUNNING;

      swtch(&(c->scheduler), next->context);
      switchkvm();

      next->stride.pass += get_stride(next);
      c->proc = 0;
    }

    release(&ptable.lock);
  }
}

// reset all pass
void reset_pass(void)
{
  struct proc *p;

  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->state != RUNNABLE || p->mode != STRIDE)
      continue;
    p->stride.pass = 0;
  }
  mlfqinfo.qstride.pass = 0;
}

// Inquires to obtain cpu share (%)
int set_cpu_share(int share)
{
  struct proc *cur = myproc();
  struct proc *p;

  acquire(&ptable.lock);

  if (mlfqinfo.usingticket + share > 80) // MLFQ should occupy at least 20%
  {
    release(&ptable.lock);
    cprintf("exception handling : MLFQ should occupy CPU share at least 20\n");
    return -1;
  }

  mlfqinfo.usingticket += share;

  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->pid == cur->pid) /* project 2 */
    {
      p->mode = STRIDE;
      p->stride.ticket = share;
    }
  }
  reset_pass();

  release(&ptable.lock);
  return 0;
}

// Enter scheduler.  Must hold only ptable.lock
// and have changed proc->state. Saves and restores
// intena because intena is a property of this
// kernel thread, not this CPU. It should
// be proc->intena and proc->ncli, but that would
// break in the few places where a lock is held but
// there's no process.
void sched(void)
{
  int intena;
  struct proc *p = myproc();

  if (!holding(&ptable.lock))
    panic("sched ptable.lock");
  if (mycpu()->ncli != 1)
    panic("sched locks");
  if (p->state == RUNNING)
    panic("sched running");
  if (readeflags() & FL_IF)
    panic("sched interruptible");
  intena = mycpu()->intena;
  swtch(&p->context, mycpu()->scheduler);
  mycpu()->intena = intena;
}

// Give up the CPU for one scheduling round.
void yield(void)
{
  acquire(&ptable.lock); //DOC: yieldlock
  myproc()->state = RUNNABLE;
  sched();
  release(&ptable.lock);
}

// A fork child's very first scheduling by scheduler()
// will swtch here.  "Return" to user space.
void forkret(void)
{
  static int first = 1;
  // Still holding ptable.lock from scheduler.
  release(&ptable.lock);

  if (first)
  {
    // Some initialization functions must be run in the context
    // of a regular process (e.g., they call sleep), and thus cannot
    // be run from main().
    first = 0;
    iinit(ROOTDEV);
    initlog(ROOTDEV);
  }

  // Return to "caller", actually trapret (see allocproc).
}

// Atomically release lock and sleep on chan.
// Reacquires lock when awakened.
void sleep(void *chan, struct spinlock *lk)
{
  struct proc *p = myproc();

  if (p == 0)
    panic("sleep");

  if (lk == 0)
    panic("sleep without lk");

  // Must acquire ptable.lock in order to
  // change p->state and then call sched.
  // Once we hold ptable.lock, we can be
  // guaranteed that we won't miss any wakeup
  // (wakeup runs with ptable.lock locked),
  // so it's okay to release lk.
  if (lk != &ptable.lock)
  {                        //DOC: sleeplock0
    acquire(&ptable.lock); //DOC: sleeplock1
    release(lk);
  }
  // Go to sleep.
  p->chan = chan;
  p->state = SLEEPING;

  sched();

  // Tidy up.
  p->chan = 0;

  // Reacquire original lock.
  if (lk != &ptable.lock)
  { //DOC: sleeplock2
    release(&ptable.lock);
    acquire(lk);
  }
}

//PAGEBREAK!
// Wake up all processes sleeping on chan.
// The ptable lock must be held.
static void
wakeup1(void *chan)
{
  struct proc *p;

  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if (p->state == SLEEPING && p->chan == chan)
      p->state = RUNNABLE;
}

// Wake up all processes sleeping on chan.
void wakeup(void *chan)
{
  acquire(&ptable.lock);
  wakeup1(chan);
  release(&ptable.lock);
}

// Kill the process with the given pid.
// Process won't exit until it returns
// to user space (see trap in trap.c).
int kill(int pid)
{
  // cprintf("kill()\n");
  struct proc *p;

  acquire(&ptable.lock);
  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->pid == pid && p->tid == 0) /* project 2 */
    {
      p->killed = 1;
      // Wake process from sleep if necessary.
      if (p->state == SLEEPING)
        p->state = RUNNABLE;
      release(&ptable.lock);
      return 0;
    }
  }
  release(&ptable.lock);
  return -1;
}

//PAGEBREAK: 36
// Print a process listing to console.  For debugging.
// Runs when user types ^P on console.
// No lock to avoid wedging a stuck machine further.
void procdump(void)
{
  static char *states[] = {
      [UNUSED] "unused",
      [EMBRYO] "embryo",
      [SLEEPING] "sleep ",
      [RUNNABLE] "runble",
      [RUNNING] "run   ",
      [ZOMBIE] "zombie"};
  int i;
  struct proc *p;
  char *state;
  uint pc[10];

  for (p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if (p->state == UNUSED)
      continue;
    if (p->state >= 0 && p->state < NELEM(states) && states[p->state])
      state = states[p->state];
    else
      state = "???";
    cprintf("%d %s %s", p->pid, state, p->name);
    if (p->state == SLEEPING)
    {
      getcallerpcs((uint *)p->context->ebp + 2, pc);
      for (i = 0; i < 10 && pc[i] != 0; i++)
        cprintf(" %p", pc[i]);
    }
    cprintf("\n");
  }
}

int getppid(void)
{
  return myproc()->parent->pid;
}

// *-------------* PROJECT 02 *-------------*
int thread_create(thread_t* thread, void* (*start_routine)(void *), void* arg)
{
  // cprintf("thread_create()\n");
  int i;
  uint sbase, sz, sp;
  struct proc *np;
  struct proc *curproc = myproc();
  struct proc *masterproc;

  // Allocate process.
  if((np = allocproc()) == 0){
      return -1;
  }

  if (curproc->masterproc == 0) // curproc is process
  {
    if (curproc->tid != 0) // error
    {
      cprintf("thread create error !\n");
      return -1;
    }
    masterproc = curproc;
    np->masterproc = masterproc; // curproc is thread
  }
  else
  {
    if (curproc->tid == 0) // error
    {
      cprintf("thread create error !\n");
      return -1;
    }
    masterproc = curproc->masterproc;
    np->masterproc = masterproc;
  }
  
  np->pid = masterproc->pid;
  np->parent = 0;
  *np->tf = *masterproc->tf;
  np->mode = masterproc->mode;
  if(np->mode == STRIDE)
  {
    np->stride.ticket = masterproc->stride.ticket;
    reset_pass();
  }

  acquire(&ptable.lock);

  np->pgdir = masterproc->pgdir;
  masterproc->tcnt++;
  // cprintf("*%d*\n", masterproc->tcnt);
  np->tid = masterproc->tcnt;
  *thread = np->tid;


  if (masterproc->freelist.num == 0)
  {
    sbase = masterproc->sz;
    masterproc->sz += PGSIZE * 2;
  }
  else if (masterproc->freelist.num > 0)
  {
    sbase = masterproc->freelist.freespace[masterproc->freelist.num - 1];
    masterproc->freelist.num -= 1;
    // cprintf("== NUM : %d ==\n", masterproc->freelist.num);
  }
  else
  {
    cprintf("thread create error !\n");
    return -1;
  }

  np->stackbase = sbase;


  if((sz = allocuvm(np->pgdir, sbase, sbase + 2*PGSIZE)) == 0)
  {
    np->state = UNUSED;  
    return -1;
  }

  release(&ptable.lock);

  safestrcpy(np->name, masterproc->name, sizeof(masterproc->name));

  sp = sz - 4;
  *((uint*)sp) = (uint)arg;
  sp -= 4;
  *((uint*)sp) = 0xffffffff;

  np->sz = sz;
  np->tf->eip = (uint)start_routine;
  np->tf->esp = sp;

  for(i = 0; i < NOFILE; i++)
    if(masterproc->ofile[i])
      np->ofile[i] = filedup(masterproc->ofile[i]);
  np->cwd = idup(masterproc->cwd);

  // Make runnable
  acquire(&ptable.lock);

  np->state = RUNNABLE;
  nextpid -= 1;
  
  release(&ptable.lock);
  
  return 0;
}

void thread_exit(void* retval)
{
  // cprintf("thread_exit()\n");
  // similar to exit()
  struct proc *curproc = myproc();
  int fd;

  // Close all open files.
  for (fd = 0; fd < NOFILE; fd++)
  {
    if (curproc->ofile[fd])
    {
      fileclose(curproc->ofile[fd]);
      curproc->ofile[fd] = 0;
    }
  }

  begin_op();
  iput(curproc->cwd);
  end_op();
  curproc->cwd = 0;

  acquire(&ptable.lock);

  curproc->retval = retval;

  // Parent might be sleeping in wait().
  wakeup1(curproc->masterproc);

  // Jump into the scheduler, never to return.
  curproc->state = ZOMBIE;
  sched();
  panic("zombie exit");
}

int
thread_join(thread_t thread, void** retval)
{
  // cprintf("thread_join()\n");
  struct proc *p;
  struct proc *curproc = myproc();
  
  if(curproc->masterproc != 0)
    return -1;
  

  if(curproc->killed)
    return -1;
  

  acquire(&ptable.lock);

  for(;;){
    // Scan through table looking for exited threads.
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    {
      if(p->tid != thread)
        continue;
      
      if(p->pid != curproc->pid)
        continue;
      
      if(p->masterproc != curproc)
      {
        release(&ptable.lock);
        return -1;
      }

      if(p->state == ZOMBIE)
      {
        // Found one.
        //return_resource(p);
        kfree(p->kstack);
        p->kstack = 0;

        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        p->state = UNUSED;

        // *-------------* PROJECT 02 *-------------*
        *retval = p->retval;
        p->tid = 0;
        p->masterproc->freelist.freespace[p->masterproc->freelist.num] = p->stackbase;
        p->masterproc->freelist.num += 1;
        // cprintf("== NUM : %d ==\n", p->masterproc->freelist.num);
        p->masterproc->tcnt--;
        p->masterproc = 0;
        // ******************************************

        deallocuvm(p->pgdir, p->sz, p->stackbase);
        // p->stackbase = 0;

        release(&ptable.lock);
        return 0;
      }
    }

    // Wait for thread to exit.  (See wakeup1 call in proc_exit.)
    sleep(curproc, &ptable.lock);  //DOC: wait-sleep
  }
}

void thread_kill(struct proc* curproc)
{
  struct proc* p;

  acquire(&ptable.lock);

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if(p->pid != curproc->pid)
      continue;
    
    if(p != curproc)
    {
      p->killed = 1;
      p->chan = 0;
      p->state = SLEEPING;
    }
  }
  release(&ptable.lock);

}

void thread_wakeup(struct proc* curproc)
{
  // cprintf("thread_wakeup\n");
  struct proc* p;
  int left = 0;

  acquire(&ptable.lock);

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    if(p->pid != curproc->pid)
      continue;

    if(p != curproc)
    {
      if(p->parent != 0)
      {
        left = 1;
        p->parent = curproc;
      }
      p->state = RUNNABLE;
    }
  }
  release(&ptable.lock);

  if(left == 1)
  {
    // cprintf("wait()\n");
    wait();
  }
}


int xem_init(xem_t* semaphore)
{
  semaphore->sema = 1;
  semaphore->waitnum = 0;
  semaphore->front = 0;
  semaphore->rear = 0;

  initlock(&semaphore->lock, "semaphore");

  if(semaphore->sema != 1)
  {
    cprintf("xem_init error\n");
    return -1;
  }
  return 0;
}

int xem_wait(xem_t* semaphore)
{
  struct proc *curproc = myproc();

  acquire(&semaphore->lock);
  semaphore->sema--;

  if(semaphore->sema < 0)
  {
    semaphore->list[semaphore->rear] = curproc;
    semaphore->waitnum++;
    semaphore->rear += 1;
    if (semaphore->rear == NPROC)
      semaphore->rear = 0;
    
    sleep(curproc, &semaphore->lock);
  }
  release(&semaphore->lock);

  return 0;
}


int xem_unlock(xem_t* semaphore)
{
  acquire(&semaphore->lock);
  semaphore->sema++;

  if(semaphore->sema <= 0)
  {
    wakeup(semaphore->list[semaphore->front]);
    semaphore->list[semaphore->front] = 0;
    semaphore->waitnum--;
    semaphore->front += 1;
    if(semaphore->front == NPROC)
      semaphore->front = 0;
  }
  release(&semaphore->lock);

  return 0;
}

int rwlock_init(rwlock_t *rwlock)
{
  rwlock->readers = 0;
  xem_init(&rwlock->lock);
  xem_init(&rwlock->writelock);

  return 0;
}

int rwlock_acquire_readlock(rwlock_t *rwlock)
{
  xem_wait(&rwlock->lock);
  rwlock->readers++;
  if(rwlock->readers == 1)
    xem_wait(&rwlock->writelock);
  xem_unlock(&rwlock->lock);

  return 0;
}

int rwlock_acquire_writelock(rwlock_t *rwlock)
{
  xem_wait(&rwlock->writelock);

  return 0;
}

int rwlock_release_readlock(rwlock_t *rwlock)
{
  xem_wait(&rwlock->lock);
  rwlock->readers--;
  if(rwlock->readers == 0)
    xem_unlock(&rwlock->writelock);
  xem_unlock(&rwlock->lock);

  return 0;
}

int rwlock_release_writelock(rwlock_t *rwlock)
{
  xem_unlock(&rwlock->writelock);

  return 0;
}