#include <stdio.h>
#include <stdlib.h>


char *get_line(FILE *fp, char *buffer, size_t size) {
    if (fp == NULL || buffer == NULL) {
        return NULL;
    }
    return fgets(buffer, size, fp);
}