#include <pthread.h>

typedef struct zemaphore {
    pthread_mutex_t lk;
    pthread_cond_t cv;
    int vl;
} zem_t;

void zem_init(zem_t *, int);
void zem_up(zem_t *);
void zem_down(zem_t *);
