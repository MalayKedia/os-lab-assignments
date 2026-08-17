#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens */
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

void free_tokens(char **tokens){
    for (int i = 0; tokens[i] != NULL; i++){
        free(tokens[i]);
    }
    free(tokens);
}

void run_cmd(char **tokens){
    int pipe_found = 0;
    for (int index=0; index<MAX_NUM_TOKENS && tokens[index]!=NULL; index++){
        if (strcmp(tokens[index], "|")==0){
            pipe_found = 1;

            tokens[index] = NULL;
            int pipefd[2];
            pipe(pipefd);

            int forkID1 = fork();

            if (forkID1 < 0) {
                printf("Error: Fork failed");
            }
            else if (forkID1 == 0){
                dup2(pipefd[1], 1);
                close(pipefd[0]);
                close(pipefd[1]);

                execvp(tokens[0], tokens);
                printf("Error: Command exectution failed\n");
                exit(1);
            }
            else if (forkID1 > 0){
                int forkID2 = fork();

                if (forkID2 < 0){
                    printf("Error: Fork failed");
                }
                else if (forkID2 == 0){
                    dup2(pipefd[0], 0);
                    close(pipefd[0]);
                    close(pipefd[1]);
                    run_cmd(tokens+index+1);
                    exit(1);
                }
                else {
                    close(pipefd[0]);
                    close(pipefd[1]);

                    waitpid(forkID1, NULL, 0);
                    waitpid(forkID2, NULL, 0);

                    return;
                }
            }
        }
    }
    if (!pipe_found){
        int forkID = fork();

        if (forkID < 0){
            printf("Error: Fork failed");
        }
        else if (forkID == 0){
            execvp(tokens[0], tokens);
            printf("Error: Command exectution failed\n");
            exit(1);
        }
        else if (forkID > 0){
            waitpid(forkID, NULL, 0);
        }
    }
}


void run_frg(char **tokens){
    if (strcmp(tokens[0], "exit") == 0) {
        if (tokens[1] != NULL){
            printf("Error: arguments provided after exit\n");
            return;
        }
        free_tokens(tokens);
        kill(-getpgid(0), SIGKILL);
        exit(0);
    }
    if (strcmp(tokens[0], "cd") == 0) {
        int successCode = chdir(tokens[1]);
        if (successCode != 0){
            printf("Error: cd exectution failed\n");
        }
    }
    else {
        run_cmd(tokens);
    }
}


void run_bkg(char **tokens){
    if (strcmp(tokens[0], "exit") == 0) {
        printf("Error: exit execution not possible in background\n");
    }
    if (strcmp(tokens[0], "cd") == 0) {
        printf("Error: cd execution not possible in background\n");
    }
    else {
        int forkID = fork();

        if (forkID < 0)
        {
            printf("Error: Fork failed");
        }
        else if (forkID == 0)
        {
            execvp(tokens[0], tokens);
            printf("Error: Command exectution failed\n");
            exit(1);
        }
    }
}



int main(int argc, char *argv[])
{
    char line[MAX_INPUT_SIZE];
    char **tokens;

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

        while(1){
			int pid_reaped = waitpid(-1, NULL, WNOHANG);
			if (pid_reaped <= 0) break;
			else printf("[%d] Background process finished\n", pid_reaped);
		}

        int start_index = 0;
        for (int index=0; index<MAX_NUM_TOKENS && tokens[index]!=NULL; index++){
            if (strcmp(tokens[index], "&")==0){
                tokens[index] = NULL;
                run_bkg(tokens+start_index);
                start_index=index+1;
            }
        }
        if (tokens[start_index] == NULL) continue;
        run_frg(tokens+start_index);

        // Freeing the allocated memory
        free_tokens(tokens);
    }
    return 0;
}
