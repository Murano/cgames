//
// Created by timur on 02.12.18.
//

#ifndef LINUX_SYSTEM_PROGRAMMING_RUN_H
#define LINUX_SYSTEM_PROGRAMMING_RUN_H

#define DB_PATH "../chapter3/data"

#define COMMAND_GET "GET"
#define COMMAND_SET "SET"
#define SET 1
#define GET 2
#define COMMAND_LEN 4

#define MAX_KEY_SIZE 10
#define MAX_VALUE_SIZE 100

struct Data {
    char key[MAX_KEY_SIZE];
    char value[MAX_VALUE_SIZE];
};

struct DataV {
    char* key;
    char* value;
};

int run();

#endif //LINUX_SYSTEM_PROGRAMMING_RUN_H
