#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <string.h>
#include "run.h"

#define BUFFSIZE 64

int run() {
    struct pollfd fds[1];

    char buff[BUFFSIZE];
    int fd;
    int ret;
    ssize_t wr;
    ssize_t rd;

    size_t current_len = 0;

    fd = open("../chapter2/data", O_APPEND | O_RDWR | O_CREAT, 0644);

    if (fd == -1) {
        perror("open");
        return 1;
    }

    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    for (;;) {
        ret = poll(fds, 1, -1);
        if (ret == -1) {
            perror("poll");
            return 1;
        }

        if (fds[0].revents & POLLIN) {
            memset(buff,'\0',BUFFSIZE);
            rd = read(STDIN_FILENO, &buff, BUFFSIZE);

            current_len = strlen(buff);
            if (current_len > BUFFSIZE) {
                return 1;
            }

            if (rd == -1) {
                perror("read");
                return 1;
            }

            wr = write(fd, &buff, current_len);

            if (wr == -1) {
                perror("write");
                return 1;
            }
        }
    }
}