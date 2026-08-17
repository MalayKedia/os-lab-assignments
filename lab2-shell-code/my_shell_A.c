#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

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

int main(int argc, char *argv[])
{
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

        if (tokens[0] == NULL) continue;

        if (strcmp(tokens[0], "cd") == 0) {
            int successCode = chdir(tokens[1]);
            if (successCode != 0)
                printf("Error: cd exectution failed\n");
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
            else if (forkID > 0)
            {
                waitpid(forkID, NULL, 0);
            }
        }

        // Freeing the allocated memory
        for (i = 0; tokens[i] != NULL; i++)
        {
            free(tokens[i]);
        }
        free(tokens);
    }
    return 0;
}
