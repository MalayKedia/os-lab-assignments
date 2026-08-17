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
#include <mqueue.h>
#include <time.h>
#include <errno.h>

#define ITERS 100
#define SLEEPTIME 100

int main()
{
	const int SIZE = 4096;
	const char *name = "OS";

	int shm_fd;
	void *ptr;

	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd, SIZE);

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	void *ptr_base = ptr;

	if (ptr == MAP_FAILED){
		printf("Map failed\n");
		return -1;
	}

	mqd_t mq;
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 100;
	attr.mq_msgsize = sizeof(int) + 1;
	attr.mq_curmsgs = 0;

	// mq = mq_open("/myqueue", O_CREAT | O_RDWR, 0666, &attr);
	// mq_close(mq);
	mq = mq_open("/myqueue3", O_CREAT | O_RDONLY, 0666, &attr);

	if (mq == (mqd_t) -1) {
    perror("Error creating message queue");
    exit(-1);
	}

	/**
	 * Now write to the shared memory region. *
	 * Note we must increment the value of ptr after each write.
	 */

	const char *message1 = "freeeee";
	for (int i = 0; i < 512; i++){
		sprintf(ptr, "%s", message1);
		ptr += strlen(message1);
	}

	const char *message2 = "OSisFUN";
	char offset[sizeof(int) + 1];

	while (1)
	{
		mq_receive(mq, offset, sizeof(int) + 1, NULL);
		// printf("%d", mq_receive(mq, offset, sizeof(int)+1, NULL));
		printf("offset: %d\n", atoi(offset));
		if (atoi(offset) != 0)
		{
			exit(1);
		}
		// sprintf((char *)(ptr_base + atoi(offset)), "%s", message2);
	}

	return 0;
}
