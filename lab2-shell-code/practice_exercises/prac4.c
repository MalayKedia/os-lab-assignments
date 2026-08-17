#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int forkID = fork();

    if (forkID == 0)
    {
        // printf("I am the child with pid %d and ppid %d\n", getpid(), getppid());
        char *args[2];

        if (argc != 3)
        {
            fprintf(stderr, "Incorrect number of arguments\n");
            exit(1);
        }
        args[0] = argv[1];
        args[1] = argv[2];

        execvp(args[0], args);

    }
    else if (forkID > 0)
    {
        int rcode;
        wait(&rcode);
        // printf("I am the parent with pid %d\n", getpid());

        if (rcode == 0) printf("Command executed successfully\n");
        else fprintf(stderr, "Command failed\n");
    }
    else
    {
        fprintf(stderr, "Fork failed\n");
    }
    return 0;
}