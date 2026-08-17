#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include "zemaphore.h"

#define NUM_THREADS 3
#define NUM_ITER 10

zem_t zm0, zm1, zm2;

void *justprint(void *data)
{
  int thread_id = *((int *)data);

  for(int i=0; i < NUM_ITER; i++)
    {
      if (thread_id==0) zem_down(&zm0);
      else if (thread_id==1) zem_down(&zm1);
      else zem_down(&zm2);

      printf("This is thread %d\n", thread_id);

      if (thread_id==0) zem_up(&zm1);
      else if (thread_id==1) zem_up(&zm2);
      else zem_up(&zm0);
      
    }
  return 0;
}

int main(int argc, char *argv[])
{

  pthread_t mythreads[NUM_THREADS];
  int mythread_id[NUM_THREADS];

  zem_init(&zm0, 1);
  zem_init(&zm1, 0);
  zem_init(&zm2, 0);
  
  for(int i =0; i < NUM_THREADS; i++)
    {
      mythread_id[i] = i;
      pthread_create(&mythreads[i], NULL, justprint, (void *)&mythread_id[i]);
    }
  
  for(int i =0; i < NUM_THREADS; i++)
    {
      pthread_join(mythreads[i], NULL);
    }
  
  return 0;
}
