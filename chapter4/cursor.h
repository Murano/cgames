//
// Created by timur on 27.12.18.
//

#ifndef LINUX_SYSTEM_PROGRAMMING_CURSOR_H
#define LINUX_SYSTEM_PROGRAMMING_CURSOR_H

#include "run.h"


typedef struct Cursor {
    char *addr;
    int length;
} Cursor;

Cursor* cursor_create(char *);
Data* cursor_get(Cursor *cursor, char *key);
void cursor_set(Cursor*, char*, char*);
int cursor_flush(Cursor*);


#endif //LINUX_SYSTEM_PROGRAMMING_CURSOR_H
