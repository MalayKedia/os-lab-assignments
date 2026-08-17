#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  rw->lk_mutex = PTHREAD_MUTEX_INITIALIZER;
  rw->reader_cv = PTHREAD_COND_INITIALIZER;
  rw->writer_cv = PTHREAD_COND_INITIALIZER;
  rw->writer_pres = false;
  rw->readers_pres = 0;
}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&rw->lk_mutex);

  while (rw->writer_pres){
    pthread_cond_wait(&rw->reader_cv, &rw->lk_mutex);
  }

  rw->readers_pres ++;
  pthread_mutex_unlock(&rw->lk_mutex);
}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  pthread_mutex_lock(&rw->lk_mutex);

  rw->readers_pres --;
  if (rw->readers_pres==0) pthread_cond_broadcast(&rw->writer_cv);
  pthread_mutex_unlock(&rw->lk_mutex);
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&rw->lk_mutex);

  while (rw->writer_pres || rw->readers_pres!=0){
    pthread_cond_wait(&rw->writer_cv, &rw->lk_mutex);
  }

  rw->writer_pres = true;
  pthread_mutex_unlock(&rw->lk_mutex);
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  pthread_mutex_lock(&rw->lk_mutex);

  rw->writer_pres = false;
  pthread_cond_broadcast(&rw->writer_cv);
  pthread_cond_broadcast(&rw->reader_cv);

  pthread_mutex_unlock(&rw->lk_mutex);
}
