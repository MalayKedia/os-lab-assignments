/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the consumer process
 *
 * Figure 3.18
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#include<string.h>

#define N 1000

#define fifo_prod "fifo-prd"

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main()
{
	const char *name = "OS";
	const int SIZE = 4096;

	const char *message1= "freeeee";
	const char *message2= "OSisFUN";

	int shm_fd;
	void *ptr;

	/* open the shared memory segment */
	shm_fd = shm_open(name, O_RDWR, 0666);
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

	/* now read from the shared memory region */
	int fd_fifo = open(fifo_prod, O_WRONLY);

	for (int i=0; i<N; i++){
		// printf("Iter no %d\n", i);
		int index = 0;
		while (1){
			// printf("Index %d\n", index);
			if (strcmp(ptr+8*index, message2)==0){
				printf("%d - %s\n", i, (char*)(ptr+8*index));
				strcpy(ptr+8*index, message1);
				
				int n = write(fd_fifo, (char*)&index, 4);
				if (n<=0) error("ERROR");
				break;
			}
			index = (index+1) % 512;
		}
	}

	int ended = -100;
	int n = write(fd_fifo, (char*)&ended, 4);
	if (n<=0) error("ERROR");
	

	close(fd_fifo);

	/* remove the shared memory segment */
	if (shm_unlink(name) == -1) {
		printf("Error removing %s\n",name);
		exit(-1);
	}

	return 0;
}
