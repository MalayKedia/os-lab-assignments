#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    int forkID = fork();

    if (forkID > 0){
        sleep(5);
        kill(forkID, SIGKILL);
    }
    else if (forkID == 0){
        printf("Child is going to sleep\n");
        sleep(1000);
    }
    else{
        fprintf(stderr, "Fork failed\n");
    }
    
    exit(0);
}