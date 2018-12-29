//
// Created by timur on 16.12.18.
//

#ifndef LINUX_SYSTEM_PROGRAMMING_RUN_H
#define LINUX_SYSTEM_PROGRAMMING_RUN_H

#define COMMAND_GET "GET"
#define COMMAND_SET "SET"
#define BACKLOG 5
#define MAX_EVENTS 10
#define DB_SIZE 4096
#define DB_PATH "../chapter4/data"

typedef struct Data {
    unsigned char key[10];
    unsigned char value[50];
} Data;

int run();


#endif //LINUX_SYSTEM_PROGRAMMING_RUN_H
