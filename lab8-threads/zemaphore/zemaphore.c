#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include "zemaphore.h"

void zem_init(zem_t *s, int value) {
    s->lk = PTHREAD_MUTEX_INITIALIZER;
    s->cv = PTHREAD_COND_INITIALIZER;
    s->vl = value;
}

void zem_down(zem_t *s) {
    pthread_mutex_lock(&s->lk);

    s->vl--;
    if (s->vl<0) pthread_cond_wait(&s->cv, &s->lk);

    pthread_mutex_unlock(&s->lk);
}


void zem_up(zem_t *s) {
    pthread_mutex_lock(&s->lk);

    if (s->vl<0) pthread_cond_signal(&s->cv);
    s->vl++;
    
    pthread_mutex_unlock(&s->lk);
}
