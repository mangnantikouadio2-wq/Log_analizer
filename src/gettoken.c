#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int get_status(const char *buffer,char *status) {
    int c = 0;
    while(c < 3 && *buffer >= '0' && *buffer <= '9') {
        status[c++] = *buffer++;
    }
    status[c] = '\0';
    return 1;
}

int get_ip(const char **buffer, char *IP) {
    int c = 0;
    while(c < 15 && **buffer != ' ' && **buffer != '\0') {
        IP[c++] = *(*buffer)++;
    }
    IP[c] = '\0';
    return 1;
}

int get_timestamp(const char **buffer, char *timestamp) {
    int c = 0;
    while(c < 31 && **buffer != ' ' && **buffer != '\0') {
        timestamp[c++] = *(*buffer)++;
    }
    timestamp[c] = '\0';
    return 1;
}

int get_method(const char **buffer, char *method) {
    int c = 0;
    while(c < 9 && **buffer != ' ' && **buffer != '\0') {
        method[c++] = *(*buffer)++;
    }
    method[c] = '\0';
    return 1;
}

int get_link(const char **buffer, char *URL) {
    int c = 0;
    while(c < 255 && **buffer != ' ' && **buffer != '\0') {
        URL[c++] = *(*buffer)++;
    }
    URL[c] = '\0';
    return 1;
}