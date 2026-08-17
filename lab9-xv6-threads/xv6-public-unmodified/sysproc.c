#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
/////////////////// New addition ///////////////////////
#include "barrier.h"
/////////////////// End of new addition ///////////////////////

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
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
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
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

/////////////////// New addition ///////////////////////
int sys_thread_create(void){
  uint *threadId;
  void *(*start_routine)(void*);
  void *arg;

  if(argptr(0, (char**)&threadId, sizeof(uint*)) < 0)
    return -1;
  if(argptr(1, (char**)&start_routine, sizeof(void*)) < 0)
    return -1;
  if(argptr(2, (char**)&arg, sizeof(void*)) < 0)
    return -1;

  return thread_create(threadId, start_routine, arg);
}

int sys_thread_exit(void){
  thread_exit();
  return 1;
}

int sys_thread_join(void){
  int threadID;

  if (argint(0, &threadID) < 0){
    return -1;
  }
  return thread_join((uint)threadID);
}

int sys_barrier_init(void)
{
  int N;
  if(argint(0, &N) < 0)
    return -1;
  return barrier_init(N);
}

int sys_barrier_check(void)
{
  return barrier_check();
}

int sys_waitpid(void)
{
  int pid;
  if (argint(0, &pid) < 0)
    return -1;
  return waitpid(pid);
}

////////////////// End of new addition /////////////////
/////////// Parts D and E of threads lab/////////
int sys_sleepChan(void) {
  int chanID;
  if(argint(0, &chanID) < 0)
    return -1;

  sleepChan(chanID);
  return 0;
}

int sys_getChannel(void) {
  return getChannel();
}

int sys_sigChan(void) {
  int chanID;
  if(argint(0, &chanID) < 0)
    return -1;

  sigChan(chanID);
  return 0;
}

int sys_sigOneChan(void) {
  int chanID;
  if(argint(0, &chanID) < 0)
    return -1;

  sigOneChan(chanID);
  return 0;
}
/////////// End of Parts D and E of threads lab/////////

