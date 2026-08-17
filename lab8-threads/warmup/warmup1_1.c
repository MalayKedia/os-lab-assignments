#include<stdio.h>
#include <pthread.h>

int counter=0;

void* increment(void*){
    for (int i=0; i<1000; i++) counter++;
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