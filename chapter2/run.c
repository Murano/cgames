#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <string.h>
#include "run.h"

#define TIMEOUT 5
#define BUFFSIZE 64

int run() {
    struct pollfd fds[2];

    char buff[BUFFSIZE];
    int fd;
    int ret;
    ssize_t wr;
    ssize_t rd;
    int flag = 0;
    size_t current_len = 0;

    fd = open("../chapter2/data", O_APPEND | O_RDWR | O_CREAT, 0644);

    if (fd == -1) {
        perror("open");
    }


    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    fds[1].fd = fd;
    fds[1].events = POLLOUT;

    for (;;) {
        ret = poll(fds, 2, TIMEOUT * 10000);
        if (ret == -1) {
            perror("poll");
            return 1;
        }

        if (fds[0].revents & POLLIN && flag == 0) {
            memset(buff,'\0',BUFFSIZE);
            rd = read(STDIN_FILENO, &buff, BUFFSIZE);

            current_len = strlen(buff);

            if (rd == -1) {
                perror("read");
                return 1;
            }

            flag = 1;
        }

        if (fds[1].revents & POLLOUT && flag == 1) {
            wr = write(fd, &buff, current_len);

            if (wr == -1) {
                perror("write");
                return 1;
            }

            flag = 0;
        }
    }
}