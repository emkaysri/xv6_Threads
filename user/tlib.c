#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"


#define PGSIZE		4096



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
  getlock(&lock);
  acquire(&lock);

   void *stack = malloc(PGSIZE);
   if (stack == NULL) return -1;

  int ret = clone(start_routine, arg, stack);
  release(&lock);
  return ret;
}

int thread_join(){
  struct lock_t lock;
  getlock(&lock);
  acquire(&lock);
  void *stack = malloc(PGSIZE);
  if (stack == NULL) return -1;
  int ret = join(&stack);
  if(ret != -1){
    free(stack);
    release(&lock);
    return ret;
  } else  {
    release(&lock);
    return -1;
  }
}
