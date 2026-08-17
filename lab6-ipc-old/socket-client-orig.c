#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>

#define SOCK_PATH "unix_socket_example12"
#define CHUNK_SIZE 256

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    int sockfd, portno, n;

    struct sockaddr_un serv_addr;
    char buffer[256];

    /* create socket, get sockfd handle */
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    /* fill in server address */
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCK_PATH);

    /* ask user for input */
    int filefd = open(argv[1], O_RDONLY);
    if (filefd < 0)
        error("ERROR opening file");

    /* send user message to server */
    while ((n = read(filefd, buffer, CHUNK_SIZE)) > 0)
    {
        printf("Sending chunk of size %d bytes...\n", n);
        n = sendto(sockfd, buffer, n, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    }

    // Close the file and socket
    close(filefd);
    close(sockfd);
    return 0;
}
