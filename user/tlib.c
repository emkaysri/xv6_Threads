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
  if(create(vars, vars, vars) == 0){
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
