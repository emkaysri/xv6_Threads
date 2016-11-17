#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "sysfunc.h"

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
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
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
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since boot.

int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

  //Actual call is clone(void(*fcn)(void*), void *arg, void *stack) so those are args
  // Basically copy from allocproc and fork but with adjustments so addr space/pgdir is copied from orig proc and stack is as per arg?
  //fcn is first instr to exe, ret is pidof thread
  //Adjust stuff about trap frame / registers
  //Adjust wait to account for threads vs children (WAIT FREES AS OF CHILD SO MAKE SURE IT WON'T FREE THE SHARED AS OF THREADS) - probably just add a bool int to proc that's isthread or something and don't free if isthread
  //Look at exec.c for stack allocation stuff (before clone is called, malloc a page to pass in as stack)
     //Regs to set up include eip, esp
  //See 2nd video for additional info
  return -1;
}

int
sys_join(void)
{
  //Will presumably mostly copy from wait()?
  return -1;
}

//TODO:
  //Also adjust exit() minimally for these changes
  //Create thread lib
  //Add to lib: thread_create(void(*start_routine)(void*), void*arg) which will basically be a wrapper for clone which mallocs a stack, then passes it and the args into clone()
  //Add to lib: thread_join(), which is of course a wrapper for join()
  //Add to lib: Create TYPE lock_t, and routines lock_acquire and lock_release (use x86 atomic xchg to make it work) and lock_init

