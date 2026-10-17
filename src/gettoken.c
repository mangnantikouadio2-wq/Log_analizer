#include <stdlib.h>
#include "parser.h"

int get_status(const char **buffer,char *status) {
    int c = 0;
    while(c < 3 && **buffer != ' ' && **buffer != '\0') {
        status[c++] = *(*buffer)++;
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

int get_time_zone(const char **buffer, char *time_zone) {
    int c = 0;
    while(c < 7 && **buffer != ']' && **buffer != '\0') {
        time_zone[c++] = *(*buffer)++;
    }
    time_zone[c] = '\0';
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

int get_link(const char **buffer, char *URL, size_t capacity) {
    size_t c = 0;
    while(**buffer != ' ' && **buffer != '\0' && **buffer != '"') {
        if (c + 1 >= capacity) {
            return 0;
        }
        URL[c++] = *(*buffer)++;
    }
    URL[c] = '\0';
    return 1;
}

int get_version(const char **buffer, char *version) {
    int c = 0;
    while(c < 9 && **buffer != '"' && **buffer != '\0') {
        version[c++] = *(*buffer)++;
    }
    version[c] = '\0';
    return 1;
}

/*int get_size(const char *buffer) {
        return atol(buffer);
}*/
