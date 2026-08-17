#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    printf("Hi there\n");

    int forkID = fork();

    if(forkID == 0){
        printf("I am the child\n");
        exit(0);
    } else if (forkID > 0){
        printf("I am the parent\n");
        wait(NULL);
    }
    else {
        fprintf(stderr, "Fork failed\n");
    }
    return 0;
}