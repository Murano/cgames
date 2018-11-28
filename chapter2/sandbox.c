#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include "run.h"

#define TIMEOUT 5

int sandbox_run() {
    struct pollfd fds[2];

    int fd;
    int wr;
    FILE data;
    char* word = "Hello pa\0";
    ssize_t len = 9;

    fd = open("../data", O_APPEND | O_RDWR | O_CREAT, 0644);

    if (fd == -1) {
        perror("open");
    }

    wr = (int) write(fd, word, (size_t) len);

    if (wr == -1) {
        perror("write");
    }


    return 0;
}