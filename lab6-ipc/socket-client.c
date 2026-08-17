#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <fcntl.h>

#define SOCK_PATH "unix_socket_example"

char password[256] = "EOF00_EOF00_EOF00_EOF00_EOF00_EOF00_EOF00_";

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
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
    if (argc != 2)
        error("Need filename as argument");

    int fd_file = open(argv[1], O_RDONLY);
    if (fd_file < 0)
        error("File couldnt be opened");
    while (1)
    {
        bzero(buffer, 256);
        ssize_t contents = read(fd_file, buffer, 256);
        if (contents < 0) error("ERROR reading file");

        printf("%s", buffer);
        int l = sendto(sockfd, buffer, contents, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

        if (contents == 0) break;
        if (l < 0)
            error("ERROR writing to socket");
    }

    int l = sendto(sockfd, password, 256, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (l < 0)
        error("ERROR writing to socket");

    close(sockfd);
    close(fd_file);

    return 0;
}
