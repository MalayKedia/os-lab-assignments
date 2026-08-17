#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(){
    char * args[3];
    args[0] = strdup("ls");
    args[1] = strdup("-l");
    args[2] = NULL;

    execvp(args[0], args);
}