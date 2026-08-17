#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void sigint_handler(int signal){
    printf("I will run forever\n");
}

int main(){
    signal(SIGINT, sigint_handler);

    for (;;){
        sleep(1);
    }
}
