#include "types.h"
#include "stat.h"
#include "user.h"
#include "spinlock.h"
#include "x86.h"

//lock and unlock
void lock(struct spinlock* lk)
{
//	printf(1, "--LOCK--\n");

	while(1)
	{
		if(xchg(&lk->locked, 1) == 0)
			break;
	}
}

void unlock(struct spinlock* lk)
{
//	printf(1, "**UNLOCK**\n");
	lk->locked = 0;
}

int
main(int argc, char* argv[])
{
	struct spinlock lk;

	int pid;
	
	for(int i = 0; i < 3; i++)
	{
//		lock(&lk);

		pid = fork();
		
		lock(&lk);

		if(pid > 0)
			printf(1, "Parent\n");
		else if(pid == 0)
			printf(1, "Child\n");
		else
			printf(1, "error\n");
		
		unlock(&lk);

		callyield();

		pid = wait();
		callyield();

//		unlock(&lk);
	}

	exit();
}
