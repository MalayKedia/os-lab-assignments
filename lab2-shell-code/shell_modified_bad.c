#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cassert>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

void sigint_handler(int signal)
{
}

/* Splits the string by space and returns the array of tokens
 *
 */
char **tokenize(char *line)
{
    char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
    int i, tokenIndex = 0, tokenNo = 0;

    for (i = 0; i < strlen(line); i++)
    {

        char readChar = line[i];

        if (readChar == ' ' || readChar == '\n' || readChar == '\t')
        {
            token[tokenIndex] = '\0';
            if (tokenIndex != 0)
            {
                tokens[tokenNo] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
                strcpy(tokens[tokenNo++], token);
                tokenIndex = 0;
            }
        }
        else
        {
            token[tokenIndex++] = readChar;
        }
    }

    free(token);
    tokens[tokenNo] = NULL;
    return tokens;
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sigint_handler);

    char line[MAX_INPUT_SIZE];
    char **tokens;
    int i;

    while (1)
    {
        /* BEGIN: TAKING INPUT */
        bzero(line, sizeof(line));
        printf("$ ");
        scanf("%[^\n]", line);
        getchar();

        // printf("Command entered: %s (remove this debug output later)\n", line);
        /* END: TAKING INPUT */

        line[strlen(line)] = '\n'; // terminate with new line
        tokens = tokenize(line);

        while (1)
        {
            int pid_reaped = waitpid(-1, NULL, WNOHANG);
            if (pid_reaped == 0 || pid_reaped == -1)
                break;
            else
                printf("Shell: Background process finished\n");
        }

        if (strcmp(tokens[0], "exit") == 0)
        {
            kill(0, SIGKILL);
            exit(0);
        }
        else if (strcmp(tokens[0], "cd") == 0)
        {
            int successCode = chdir(tokens[1]);

            if (successCode != 0)
                printf("Command exectution failed\n");
        }
        else
        {
            int bkgrnd = 0;

            for (int tokenID = 0; tokens[tokenID] != 0; tokenID++)
            {
                if (strcmp(tokens[tokenID], "&") == 0)
                    bkgrnd = 1;
                if (bkgrnd == 1)
                    tokens[tokenID] = tokens[tokenID + 1];
            }

            int parallel = 1, series = 1;
            int starts[64];
            int counter = 1;
            memset(starts, 0, 64 * sizeof(int));

            for (int tokenID = 0; tokens[tokenID] != 0; tokenID++)
            {
                if (strcmp(tokens[tokenID], "&&") == 0)
                {
                    series += 1;
                    tokens[tokenID] = 0;
                    starts[counter++] = tokenID + 1;
                }
                if (strcmp(tokens[tokenID], "&&&") == 0)
                {
                    parallel += 1;
                    tokens[tokenID] = 0;
                    starts[counter++] = tokenID + 1;
                }
            }
            assert(!(parallel > 1 && series > 1));

            if (bkgrnd)
            {
                int forkID = fork();
                if (forkID == 0)
                {
                    setpgid(0, 0);
                    execvp(tokens[0], tokens);
                    printf("Command exectution failed\n");
                }
            }

            if (parallel > 1)
            {
                int pids[parallel];
                for (int i = 0; i < parallel; i++)
                {
                    pids[i] = fork();
                    if (pids[i] == 0)
                    {
                        execvp(tokens[starts[i]], tokens + starts[i]);
                        printf("Command exectution failed\n");
                    }
                }
                for (int i = 0; i < parallel; i++)
                {
                    waitpid(pids[i], NULL, 0);
                }
            }

            else
            {
                int forkID = fork();

                if (forkID == 0)
                {
                    execvp(tokens[starts[i]], tokens + starts[i]);
                    printf("Command exectution failed\n");
                }
                else if (forkID > 0)
                {
                    waitpid(forkID, NULL, 0);
                }
            }
        }

        // do whatever you want with the commands, here we just print them

        // for(i=0;tokens[i]!=NULL;i++){
        // 	printf("found token %s (remove this debug output later)\n", tokens[i]);
        // }

        // Freeing the allocated memory
        for (i = 0; tokens[i] != NULL; i++)
        {
            free(tokens[i]);
        }
        free(tokens);
    }
    return 0;
}
