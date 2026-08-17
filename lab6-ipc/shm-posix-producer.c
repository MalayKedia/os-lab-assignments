/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the producer process that writes to the shared memory region.
 *
 * Figure 3.17
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

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
	const int SIZE = 4096;
	const char *name = "OS";
	const char *message1= "freeeee";
	const char *message2= "OSisFUN";

	int shm_fd;
	void *ptr;

	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE);

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	/**
	 * Now write to the shared memory region.
 	 *
	 * Note we must increment the value of ptr after each write.
	 */

	for (int i=0; i<512; i++){
		sprintf(ptr+i*8, "%s", message1);
	}

	mkfifo(fifo_prod, 0666);
	int fd_fifo = open(fifo_prod, O_RDONLY);

	for (int i=0; i<512; i++){
		sprintf(ptr+i*8, "%s", message2);
		printf("%d - %s\n", i, (char*)(ptr+i*8));
	}
	for (int i=512; i<N; i++){
		int offset = 0;
		int n = read(fd_fifo, (char*)&offset, 4);
		if (n<=0) error("Read failed in producer");

		sprintf(ptr+8*offset, "%s", message2);
		printf("%d - %s\n", i, (char*)(ptr+8*offset));
	}

	while(1){
		int done;
		read(fd_fifo, &done, sizeof(done));
		if(done == -100) break;
	}

	return 0;
}
