#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    printf("The value of N is: ");
    int N; scanf("%d", &N);

    for (int i=0; i<N; i++){
        int forkID = fork();

        if (forkID > 0){
            wait(NULL);
        }
        else if (forkID<0){
            fprintf(stderr, "Fork failed\n");
        }
    }

    printf("I am the child with pid %d\n", getpid());   
    exit(0);
}
