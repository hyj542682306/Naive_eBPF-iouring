#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <sys/sdt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "liburing.h"

#define BUFSIZE (1<<11)
#define BATSIZE (1<<5)
#define ITERSIZE 100

void test_iouring() {
	int fd = open("testIOuring", O_RDWR | O_CREAT | O_DIRECT);
	truncate(fd, 2 * BUFSIZE);

	char *buf;
	posix_memalign((void **) &buf, 512, BUFSIZE);
	memset(buf, 'G', BUFSIZE);
	for(int it = 0; it < ITERSIZE; ++it) {
		struct io_uring ring;
		struct io_uring_sqe *sqe;
		struct io_uring_cqe *cqe;
		struct io_uring_cqe **completes = malloc(sizeof(struct io_uring_cqe *) *BATSIZE);
        io_uring_queue_init(BATSIZE * 2, &ring, IORING_SETUP_SQPOLL);
        for(int bt = 0 ; bt < BATSIZE; ++bt) {
            sqe = io_uring_get_sqe(&ring);
			DTRACE_PROBE(oyiya, io_uring_in);
            if(bt & 1) io_uring_prep_read(sqe, fd, buf, BUFSIZE, 0);
            else io_uring_prep_write(sqe, fd, buf, BUFSIZE, 0);
			DTRACE_PROBE(oyiya, io_uring_out);
        }
		for(int cnt = 0; cnt != BATSIZE; cnt += io_uring_submit(&ring));
		for(int cnt = 0, done; cnt != BATSIZE; cnt += done) {
            done = io_uring_peek_batch_cqe(&ring, completes, BATSIZE);
            if(done == 0) continue;
            io_uring_cq_advance(&ring, done);
        }
        free(completes);
    }
    free(buf);
}

void test_normal() {
	int fd = open("testNormal", O_RDWR | O_CREAT | O_DIRECT);
	truncate(fd, 2 * BUFSIZE);

	char *buf;
	posix_memalign((void **) &buf, 512, BUFSIZE);
	memset(buf, 'G', BUFSIZE);
	for(int it = 0; it < ITERSIZE * BATSIZE; ++it) {
		lseek(fd, 0, SEEK_SET);
		DTRACE_PROBE(oyiya, normal_in);
		if(it & 1) read(fd, buf, BUFSIZE * BATSIZE);
		else write(fd, buf, BUFSIZE * BATSIZE);
		DTRACE_PROBE(oyiya, normal_out);
	}
	free(buf);
}

int main(int argc, char *argv[]) {
	FILE *file = fopen("bpf_pid", "w");
	fprintf(file, "%d", getpid());
	fclose(file);
	sleep(2);

	//if(strcmp(argv[0], "-normal") == 0) test_normal();
	//else test_iouring();

	test_normal();
	
	test_iouring();
	return 0;
}
