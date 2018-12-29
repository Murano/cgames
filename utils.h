//
// Created by timur on 23.12.18.
//

#ifndef LINUX_SYSTEM_PROGRAMMING_UTILS_H
#define LINUX_SYSTEM_PROGRAMMING_UTILS_H

#define COMMAND_GET "GET"
#define COMMAND_SET "SET"

#include <stdlib.h>
#include <stdio.h>

#define RETURN_FAILURE(arg) \
    perror(arg); \
    return -1;

#define FAIL(arg) \
    perror(arg); \
    exit(EXIT_FAILURE);

#define RETURN_NULL(arg) \
    perror(arg); \
    return NULL;

void str_upper(char*);

#endif //LINUX_SYSTEM_PROGRAMMING_UTILS_H
