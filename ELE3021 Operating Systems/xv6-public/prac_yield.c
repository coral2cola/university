#include "types.h"
#include "defs.h"

int
call_yield(void)
{
	yield();
//	cprintf("--called yield--\n");
	return 0;
}

int
sys_callyield(void)
{
	return call_yield();
}
