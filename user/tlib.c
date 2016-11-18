// TODO Add any additional include statements needed, create the global lock that the threads can access (held in proc state?)

#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"


#define PGSIZE		4096

struct lock_t{
  int flag;
};

void init(struct lock_t *lock) {
  lock->flag = 0;
}

void acquire(struct lock_t *lock) {
  while(xchg((uint*)(&lock->flag), 1) == 1) ;//spin-wait (do nothing)
}

void release(struct lock_t*lock) {
  lock->flag = 0;
}

int thread_create(void *start_routine, void *arg){
  struct lock_t lock;
  init(&lock); // TODO DELETE THESE LINES ONCE ACTUAL GLOBAL LOCK IS MADE
  acquire(&lock);  // TODO Once the lock that will be used is defined pass it in as arg

   /*void *stack = malloc(PGSIZE*2);
   assert(stack != NULL);
   if((uint)stack % PGSIZE)
     stack = stack + (4096 - (uint)stack % PGSIZE);*/

   void *stack = malloc(PGSIZE);
   if (stack == NULL) return -1;


  if(clone(start_routine, arg, stack) == 0){
    release(&lock);
    return 0;
  } else  {
    release(&lock);
    return -1;
  }
  /*release(&lock);
  return -1;*/
}

int thread_join(){
  struct lock_t lock;
  init(&lock); // TODO DELETE THESE LINES ONCE ACTUAL GLOBAL LOCK IS MADE
  acquire(&lock);
  void *stack = malloc(PGSIZE);
  if (stack == NULL) return -1;
  if(join(&stack) == 0){
    free(stack);
    release(&lock);
    return 0;
  } else  {
    release(&lock);
    return -1;
  }
  /*release(&lock);
  return -1;*/
}
