//
// Created by timur on 27.12.18.
//

#include <fcntl.h>
#include <sys/stat.h>
#include <memory.h>
#include <unistd.h>
#include <sys/mman.h>
#include "../utils.h"
#include "cursor.h"

struct Cursor* cursor_create(char *db_path) {
    struct Cursor *cur;
    struct stat st;
    char *p;
    int fd, len;
    len = 0;

    fd = open(db_path, O_APPEND | O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        RETURN_NULL("cursor_create: open");
    }

    if (fstat(fd, &st) == -1) {
        RETURN_NULL("cursor_create: fstat");
    }

    if (!S_ISREG(st.st_mode)) {
        RETURN_NULL("cursor_create: Db is not a file\n");
    }

    if (st.st_size < DB_SIZE) {
        char buf[DB_SIZE];
        memset(&buf, 0x00, DB_SIZE);
        if (write(fd, buf, DB_SIZE) == -1) {
            RETURN_NULL("cursor_create: write");
        }
    }

    p = mmap(NULL, DB_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) {
        RETURN_NULL("cursor_create: mmap");
    }

    if (close(fd) == -1) {
        RETURN_NULL("cursor_create: close");
    }

    cur = malloc(sizeof(Cursor));
    if (!cur) {
        RETURN_NULL("cursor_create: malloc");
    }

    cur->addr = p;

    while (strcmp(p, "") != 0) {
        ++len;
        p += sizeof(Data);
    }

    cur->length = len;

    return cur;
};

Data* cursor_get(Cursor *cursor, char *key) {
    char *p;
    p = cursor->addr;
    while (strcmp(p, "") != 0) {
        if (strcmp(p, key) == 0) {
            return (Data*)p;
        }
        p += sizeof(Data);
    }

    return NULL;
};

void cursor_set(Cursor *cursor, char *key, char *value) {
    Data *data, *p;

    if ((data = cursor_get(cursor, key)) != NULL) {
        memset(data->value, '\0', 50);
        strcpy((char *) data->value, value);
        return;
    }

    p = (Data *) (cursor->addr + sizeof(Data) * cursor->length);

    strcpy((char *) p->key, key);
    strcpy((char *) p->value, value);
    ++cursor->length;
}

int cursor_flush(Cursor* cursor) {
    if (msync((void *)cursor->addr, DB_SIZE, MS_SYNC) == -1) {
        RETURN_FAILURE("cursor_flush: msync");
    }

    return 0;
};
