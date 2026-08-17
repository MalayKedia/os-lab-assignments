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
#include <string.h>
#include <mqueue.h>


#define ITERS 100
#define SLEEPTIME 100000

int main()
{
	const char *name = "OS";
	const int SIZE = 4096;

	int shm_fd;
	void *ptr, *ptr_base;
	int i;

	/* open the shared memory segment */
	shm_fd = shm_open(name, O_RDWR, 0666);
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	ptr_base = ptr;

	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

	mqd_t mq = mq_open("/myqueue2", O_WRONLY);

	/* now read from the shared memory region */
	
	char message[sizeof(int)+1];

	for (int i=0; i<ITERS; i++){
		printf("%s\n", (char *)ptr);
		int len = strlen((char *)ptr);
		// memset(ptr, 0, len);

		sprintf(message, "%d", (int)(ptr - ptr_base));
		mq_send(mq, message, sizeof(int)+1, 0);
		printf("Msg sent\n");

		ptr += len;
		// if ptr is at the end of the string, reset ptr to the beginning
		usleep(SLEEPTIME);
	}

	mq_close(mq);	

	/* remove the shared memory segment */
	if (shm_unlink(name) == -1) {
		printf("Error removing %s\n",name);
		exit(-1);
	}

	return 0;
}
