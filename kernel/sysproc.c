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

int
sys_clone(void)
{
  char* fcn = NULL; //Don't know if making these char*s works but was looking at sys_read as reference
  char* arg = NULL;
  char* stack = NULL;
  if(argptr(0, &fcn, sizeof(void*)) < 0  // TODO Size args might need adjustment
  || argptr(1, &arg, sizeof(void*)) < 0
  || argptr(2, &stack, PGSIZE) < 0)
    return -1;
  return procclone(fcn, arg, stack);
  // Can't use allocproc inside this file so calling our fork clone inside proc.c instead
}

  //clone(void(*fcn)(void*), void *arg, void *stack)

  //fcn is first instr to exe, return is pidof thread

  //arg is arg to be passed to the method at fcn

  //Adjust stuff about trap frame / registers

  //Adjust wait to account for threads vs children (WAIT FREES AS OF CHILD SO MAKE SURE IT WON'T FREE THE SHARED AS OF THREADS) - I wrote something for this but am not 100% sure it works as desired in all places

  //Look at exec.c for stack allocation stuff (before clone is called, malloc a page to pass in as stack)
     //Regs to set up include eip, esp


int
sys_join(void)
{
  return thrjoin();  // TODO INCOMPLETE: needs to take its arg (stack) and copy the child's stack onto the address pointed to by the arg so it can be viewed after return
}

//TODO:
  //Also adjust exit() minimally for these changes maybe
