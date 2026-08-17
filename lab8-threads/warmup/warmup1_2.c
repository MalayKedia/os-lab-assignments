#include<stdio.h>
#include <pthread.h>

int counter=0;
pthread_mutex_t lock= PTHREAD_MUTEX_INITIALIZER;

void* increment(void*){
    for (int i=0; i<10000; i++) {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(){
    pthread_t t[10];

    for (int i=0; i<10; i++){
        pthread_create(&t[i], NULL, &increment, NULL);
    }

    for (int i=0; i<10; i++){
        pthread_join(t[i], NULL);
    }
    printf("Counter: %d\n", counter);
}