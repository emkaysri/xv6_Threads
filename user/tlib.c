// TODO Add any include statements needed, add a header file for this so we can use the library in other files (might need to move parts of this to there), add stuff for this and its header to user makefile

typedef struct__lock_t {
  intflag;
}lock_t;

voidinit(lock_t*lock) {
lock->flag = 0;
}

void acquire(lock_t*lock) {
while(xchg(&lock->flag, 1) == 1) ;//spin-wait (do nothing)
}

void release(lock_t*lock) {
lock->flag = 0;
}

int thread_create(void (*start_routine)(void*), void *arg){
  acquire(lock_t*lock); 
  // TODO Malloc stack and pass in as third arg
  if(clone(start_routine, arg, vars) == 0){
    release(lock_t*lock);
    return 0;
  } else  {
    release(lock_t*lock);
    return -1;
  }
}

int thread_join(){
  acquire(lock_t*lock);
  if(join(userstack) == 0){
    free(userstack);
    release(lock_t*lock);
    return 0;
  } else  {
    release(lock_t*lock);
    return -1;
  }
}
