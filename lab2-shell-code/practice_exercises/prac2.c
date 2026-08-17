#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    printf("Hi there\n");

    

    int forkID = fork();

    if(forkID == 0){
        printf("I am the child with pid %d and ppid %d\n", getpid(), getppid());
        exit(0);
    } else if (forkID > 0){
        wait(NULL);
        printf("I am the parent with pid %d\n", getpid());
    }
    else {
        fprintf(stderr, "Fork failed\n");
    }
    return 0;
}