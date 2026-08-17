#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<assert.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct arg{
    int tNo;

    pthread_cond_t* cvI;
    pthread_cond_t* cvIplus1;
    int* doneI;
    int* doneIplus1;
};

void* printer(void* a){
    struct arg s = *(struct arg*)a;

    for (int i=0; i<10; i++){
        pthread_mutex_lock(&lock);
        
        while(!*(s.doneI)) pthread_cond_wait(s.cvI, &lock);
        printf("I am thread %d\n", s.tNo);
        *(s.doneIplus1) = 1;
        *(s.doneI) = 0;
        pthread_cond_signal(s.cvIplus1);
        pthread_mutex_unlock(&lock);
    }
    

    return NULL;
}

int main(int argc, char** argv){
    if (argc!=2){
        printf("Usage ./executable N\n");
        return 0;
    }

    int N=atoi(argv[1]);

    pthread_t t[N];
    pthread_cond_t cv[N];
    struct arg a[N];

    int done[N];
    for (int i=0; i<N; i++){
        done[i]=0;
        int ret=pthread_cond_init(&cv[i], NULL);
        assert(ret==0);
    }

    done[0]=1;
    for (int i=0; i<N; i++){
        a[i].cvI = &cv[i];
        a[i].cvIplus1 = &cv[(i+1)%N];
        a[i].tNo = i;
        a[i].doneI = &done[i];
        a[i].doneIplus1 = &done[(i+1)%N];
        pthread_create(&t[i], NULL, printer, (void*)&a[i]);
    }

    for (int i=0; i<N; i++){
        pthread_join(t[i], NULL);
    }

}