#include "semaphore.h"

struct stat;
struct rtcdate;
struct xem_t;
struct rwlock_t;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int myfunction(char*);
int getppid(void);
void yield(void);
int getlev(void);
int set_cpu_share(int);
// *-------------* PROJECT 02 *-------------*
int thread_create(thread_t*, void*, void*);
void thread_exit(void*) __attribute__((noreturn));
int thread_join(thread_t, void**);
// ******************************************
// *-------------* PROJECT 03 *-------------*
int xem_init(struct xem_t*);
int xem_wait(struct xem_t*);
int xem_unlock(struct xem_t*);
int rwlock_init(struct rwlock_t*);
int rwlock_acquire_readlock(struct rwlock_t*);
int rwlock_acquire_writelock(struct rwlock_t*);
int rwlock_release_readlock(struct rwlock_t*);
int rwlock_release_writelock(struct rwlock_t*);
// ******************************************
// *-------------* PROJECT 04 *-------------*
int pread(int, void*, int, int);
int pwrite(int, void*, int, int);
// ******************************************

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);

// tsafe.c
// *-------------* PROJECT 04 *-------------*
thread_safe_guard* thread_safe_guard_init(int fd);
int thread_safe_pread(thread_safe_guard* file_guard, void* addr, int n, int off);
int thread_safe_pwrite(thread_safe_guard* file_guard, void* addr, int n, int off);
void thread_safe_guard_destroy(thread_safe_guard * file_guard);
// ******************************************