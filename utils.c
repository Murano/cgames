//
// Created by timur on 23.12.18.
//
#include <stdio.h>
#include <ctype.h>
#include "utils.h"


void str_upper(char* s) {
    while (*s != '\0') {
        *s = (char) toupper((unsigned char) *s);
        ++s;
    }
}