#include <stdio.h>
#include <string.h>
#include "parser.h"


char *get_line(FILE *fp, char *buffer, size_t size) {
    if (fp == NULL || buffer == NULL) {
        return NULL;
    }
    if (fgets(buffer, size, fp) == NULL) {
        return NULL;
    }
    size_t len = strlen(buffer);
    if (len == size - 1 && buffer[len - 1] != '\n' && !feof(fp)) {
        int c;
        while ((c = fgetc(fp)) != '\n' && c != EOF) {
        }
        return NULL;
    }
    return buffer;
}