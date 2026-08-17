/*----------xv6 sync lab----------*/
#include "types.h"
#include "x86.h"
#include "spinlock.h"
#include "defs.h"
#include "barrier.h"

//define any variables needed here
struct barrier{
  int N;
  int curr_count;
  struct spinlock lock;
};

struct barrier B;

int
barrier_init(int n)
{
  initlock(&B.lock, "barrier lock");
  B.N = n;
  B.curr_count = 0;
  return 0; 
}

int
barrier_check(void)
{
  acquire(&B.lock);
  B.curr_count++;
  if (B.curr_count < B.N){
    sleep(&B ,&B.lock);
  }
  else {
    B.curr_count = 0;
    wakeup(&B);
  }
  release(&B.lock);
  return 0;
}

/*----------xv6 sync lock end----------*/
