#include "semaphore.h" /* project 03 */

// Per-CPU state
struct cpu {
  uchar apicid;                // Local APIC ID
  struct context *scheduler;   // swtch() here to enter scheduler
  struct taskstate ts;         // Used by x86 to find stack for interrupt
  struct segdesc gdt[NSEGS];   // x86 global descriptor table
  volatile uint started;       // Has the CPU started?
  int ncli;                    // Depth of pushcli nesting.
  int intena;                  // Were interrupts enabled before pushcli?
  struct proc *proc;           // The process running on this cpu or null
};

extern struct cpu cpus[NCPU];
extern int ncpu;

//PAGEBREAK: 17
// Saved registers for kernel context switches.
// Don't need to save all the segment registers (%cs, etc),
// because they are constant across kernel contexts.
// Don't need to save %eax, %ecx, %edx, because the
// x86 convention is that the caller has saved them.
// Contexts are stored at the bottom of the stack they
// describe; the stack pointer is the address of the context.
// The layout of the context matches the layout of the stack in swtch.S
// at the "Switch stacks" comment. Switch doesn't save eip explicitly,
// but it is on the stack and allocproc() manipulates it.
struct context {
  uint edi;
  uint esi;
  uint ebx;
  uint ebp;
  uint eip;
};

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

// represent scheduling mode
enum schedule { MLFQ, STRIDE };

struct mlfq {
  int level;                  // MLFQ Level. (0 : highest priority queue, 2 : lowest priority queue)
  int prior;                  // priority in queue
  int qticks;                 // tick of proc (for RR and time allotment)
} mlfq;

struct stride{
  int ticket;                 // number of tickets
  float stride;               // stride value
  float pass;                 // pass value
} stride;

struct mlfqinfo{
  int usingticket;            // number of using tickets (max : 80)
  int hpriority;              // high priority
  int MLFQticks;              // ticks for priority boost
  struct stride qstride;      // save stride scheduler information
} mlfqinfo;

// *-------------* PROJECT 02 *-------------*
struct freelist {
  int num;                    // Number of free chunks
  uint freespace[NPROC];      // Free list
};
// ******************************************

// // *-------------* PROJECT 03 *-------------*
// typedef struct xem_t {
//   struct spinlock lock;
//   uint sema;                  // semaphore varible
//   int waitnum;                // the number of waiting threads
//   struct proc* list[64];      // waiting list (queue)
//   int front;
//   int rear;
// }xem_t;

// typedef struct rwlock_t {
//   struct xem_t lock;          // binary semaphore
//   struct xem_t writelock;     // used to allow ONE writer or MANY readers
//   int readers;                // count of readers reading in critical section
// }rwlock_t;
// // ******************************************


// Per-process state
struct proc {
  uint sz;                     // Size of process memory (bytes)
  pde_t* pgdir;                // Page table
  char *kstack;                // Bottom of kernel stack for this process
  enum procstate state;        // Process state
  int pid;                     // Process ID
  struct proc *parent;         // Parent process
  struct trapframe *tf;        // Trap frame for current syscall
  struct context *context;     // swtch() here to run process
  void *chan;                  // If non-zero, sleeping on chan
  int killed;                  // If non-zero, have been killed
  struct file *ofile[NOFILE];  // Open files
  struct inode *cwd;           // Current directory
  char name[16];               // Process name (debugging)

  enum schedule mode;          // represent scheduler mode
  struct mlfq mlfq;            // MLFQ
  struct stride stride;        // stride scheduler
  int yield; 

  // *-------------* PROJECT 02 *-------------*
  int tid;                     // Thread ID (for thread, master : 0)
  int tcnt;                    // Number of threads in LWP group (for master, thread : 0)
  struct proc* masterproc;     // Master process of LWP group
  void* retval;                // Return value of thread
  uint stackbase;              // Base address of LWP's stack
  struct freelist freelist;    // Free chunk of deallocated thread
  // ******************************************
};

// Process memory is laid out contiguously, low addresses first:
//   text
//   original data and bss
//   fixed-size stack
//   expandable heap