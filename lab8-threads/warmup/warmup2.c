#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void* printer(void* a){
    int i= *(int*)a;

    printf("I am thread %d\n", i);
}

int main(int argc, char** argv){
    if (argc!=2){
        printf("Usage ./executable N\n");
        return 0;
    }

    int N=atoi(argv[1]);

    pthread_t t[N];
    int a[N];
    for (int i=0; i<N; i++){
        a[i]=i;
        pthread_create(&t[i], NULL, printer, (void*)&a[i]);
    }

    for (int i=0; i<N; i++){
        pthread_join(t[i], NULL);
    }

}