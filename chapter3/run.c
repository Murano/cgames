#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <poll.h>
#include <stdlib.h>
#include <memory.h>
#include "run.h"

typedef struct Data DataStruct;

DataStruct read_data_from_stdin();
int search_data_in_file(FILE* , DataStruct*);
int get_command();


int run() {

    int ret, command, search_res;
    DataStruct data;
    FILE* data_file = fopen(DB_PATH, "a+");
    if (!data_file) {
        perror("Can not open db file");
        return 1;
    }

    struct pollfd fds[1];
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;

    for (;;) {
        ret = poll(fds, 1, -1);
        if (ret == -1) {
            perror("poll");
            return 1;
        }

        if (fds[0].revents & POLLIN) {
            command = get_command();
            if (command == -1) {
                perror("Bad command\n");
            } else if (command == SET) {
                data = read_data_from_stdin();
                if (!fwrite(&data, sizeof(DataStruct), 1, data_file)) {
                    perror("fwrite");
                    return 1;
                }
                fflush(data_file);
                printf("SET\n");
            } else if (command == GET) {
                search_res = search_data_in_file(data_file, &data);

                if (search_res == -1) {
                    return 1;
                } else if (search_res == 0) {
                    printf("Not found\n");
                } else {
                    printf("Found by key '%s' value '%s'\n", data.key, data.value);
                }

                printf("GET\n");
            }
        }

        fflush(stdout);

    }
}

int search_data_in_file(FILE* file, DataStruct* data) {
    size_t factor = 0;
    size_t length = sizeof(DataStruct);
    int fseek_res;
    char* key;
    char buff[MAX_KEY_SIZE];

    if (!fgets(buff, MAX_KEY_SIZE, stdin)) {
        perror("Cannot read key value from stdin");
        exit(1);
    }

    key = strtok(buff, " \n");

    if (!key) {
        perror("key does not set");
        return -1;
    }

    for (;;) {
        fseek_res = fseek(file, length * factor, SEEK_SET);

        if (fseek_res == -1) {
            perror("fseek");
            return -1;
        }

        if (!fread(data, length, 1, file)) {
            if (feof(file)) {
                return 0;
            } else {
                perror("fread");
                return -1;
            }
        }

        if (strcmp(key, data->key) == 0) {
            fseek_res = fseek(file, 0, SEEK_END);
            if (fseek_res == -1) {
                perror("fseek");
                return -1;
            }

            return 1;
        }

        factor++;
    }
}


DataStruct read_data_from_stdin() {

    DataStruct data = {
            .key = "",
            .value = ""
    };
    char buff[MAX_KEY_SIZE + MAX_VALUE_SIZE];
    char * key;
    char * value;

    if (!fgets(buff, MAX_KEY_SIZE + MAX_VALUE_SIZE, stdin)) {
        perror("Cannot read key value from stdin");
        exit(1);
    }

    key = strtok(buff, " ");
    value = strtok(NULL, "\n");

    if (!key) {
        perror("key does not set");
        exit(1);
    }

    if (!value) {
        perror("value does not set");
        exit(1);
    }

    if (strlen(key) > MAX_KEY_SIZE) {
        perror("max key size reached");
        exit(1);
    }

    if (strlen(value) > MAX_VALUE_SIZE) {
        perror("max value size reached");
        exit(1);
    }

    strcpy(data.key, key);
    strcpy(data.value, value);

    return data;


}

int get_command() {
    char command[4] = {'\0', '\0', '\0', '\0'};
    int c;
    int counter = 0;

    for (;;) {
        c = fgetc(stdin);
        if (c == EOF) {
            perror("Error occurred during reading command");
            exit(1);
        }

        if (isspace(c)) {
            if (counter > 0) {
                break;
            } else {
                continue;
            }
        }

        c = toupper(c);
        command[counter++] = (char)c;

        if (counter >= 3) {
            break;
        }
    }

    if (strcmp(COMMAND_SET, command) == 0) {
        return SET;
    } else if (strcmp(COMMAND_GET, command) == 0) {
        return GET;
    }

    return -1;
}