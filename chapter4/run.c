//
// Created by timur on 16.12.18.
//

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <string.h>
#include "../utils.h"
#include "cursor.h"
#include "run.h"


int create_listen_socket(uint16_t);
int deal_with_peer_socket(int, char*, Cursor*);

int run() {
    int sfd, cfd, epoll_fd, nfds, n;

    struct sockaddr_storage peer_addr;
    struct epoll_event ev, events[MAX_EVENTS];
    Cursor *cursor;

    char buf[INT8_MAX];
    socklen_t len_peer_addr;

    if (!(cursor = cursor_create(DB_PATH))) {
        FAIL("Can't create")
    }

    sfd = create_listen_socket(36691);
    if (sfd == -1) {
        FAIL("create_listen_socket");
    }

    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        FAIL("epoll_create1");
    }

    ev.events = EPOLLIN;
    ev.data.fd = sfd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sfd, &ev) == -1) {
        FAIL("epoll_ctl: listen_sock");
    }

    len_peer_addr = sizeof(peer_addr);

    for (;;) {
        nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            FAIL("epoll_wait");
        }

        for (n = 0; n < nfds; ++n) {
            if (events[n].data.fd == sfd) {
                cfd = accept(sfd,(struct sockaddr *) &peer_addr, &len_peer_addr);
                if (cfd == -1) {
                    FAIL("accept");
                }
//                setnonblocking(conn_sock); ??
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = cfd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, cfd,
                              &ev) == -1) {
                    FAIL("epoll_ctl: conn_sock");
                }
            } else {
                deal_with_peer_socket(events[n].data.fd, buf, cursor);
            }
        }

    }
}


int deal_with_peer_socket(int sock_fd, char *buf, Cursor *cursor) {
    memset(buf, '\0', INT8_MAX);
    char inner_buff[INT8_MAX];
    char* command;
    char* key;
    char* value;
    Data* data;

    recv(sock_fd, buf, INT8_MAX, 0);

    command = strtok(buf, " \r\n");

    if (command == NULL) {
        RETURN_FAILURE("empty command");
    }

    key = strtok(NULL, " \r\n");
    value = strtok(NULL, "\r\n");

    str_upper(command);

    if (strcmp(command, COMMAND_GET) == 0) {

        if (!(data = cursor_get(cursor, key))) {
            if (send(sock_fd, "value not found\n", 16, 0) == -1) {
                RETURN_FAILURE("send_error");
            }
        } else {
            int le = sprintf(buf, "%s\n", data->value);
            if (send(sock_fd, buf, le, 0) == -1) {
                RETURN_FAILURE("send_error");
            }
        }
    } else if (strcmp(command, COMMAND_SET) == 0) {
        if (value == NULL) {
            RETURN_FAILURE("value is not set");
        }

        cursor_set(cursor, key, value);
        if (send(sock_fd, "OK!\n", 4, 0) == -1) {
            RETURN_FAILURE("send_error");
        }

    } else {
        if (send(sock_fd, "Command not found\n", 19, 0) == -1) {
            RETURN_FAILURE("send_error");
        }
    }

    return 0;
}

int create_listen_socket(uint16_t port) {
    struct sockaddr_in my_addr;
    int res, sfd;
    int optval = 1;

    memset(&my_addr, 0, sizeof(struct sockaddr_in));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    res = inet_aton("127.0.0.1", &my_addr.sin_addr);
    if (res == 0) {
        RETURN_FAILURE("inet_aton");
    }

    sfd = socket(AF_INET, SOCK_STREAM , 6);
    if (sfd == -1) {
        RETURN_FAILURE("serv_sock");
    }

    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    res = bind(sfd, (struct sockaddr*)&my_addr, sizeof(my_addr));
    if (res == -1) {
        RETURN_FAILURE("bind");
    }

    res = listen(sfd, BACKLOG);
    if (res == -1) {
        RETURN_FAILURE("listen");
    }

    return sfd;
}