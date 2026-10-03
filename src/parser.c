#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int parse_line(const char *line, LogEntity *entry) {
    int state = OUT;
    get_ip(&line, entry->ip);
    while (*line == ' ' || *line == '-') {
        line++;
    }
    if (*line++ == '[') {
        state = IN_BRACKETS;
        line++;
        while (state == IN_BRACKETS) {
            get_timestamp(&line, entry->timestamp);
            while (*line && *line != ']') {
                line++;
            }
            if (*line == ']') {
                line++;
            }
            state = OUT;
        }
    } else {
        return 0;
    }
    while(*line == ' ') {
        line++;
    }
    if (*line == '"') {
        state = IN_QUOTES;
        line++;
        while (state == IN_QUOTES) {
            get_method(&line, entry->req.method);
            if (*line == ' ') {
                line++;
            }
            get_link(&line, entry->req.url);
            if (*line == '"') {
                line++;
            }
            state = OUT;
        }
    } else {
        return 0;
    }
    while(*line && (*line < '0' || *line > '9')) {
        line++;
    }
    entry->status = get_status(line);
    return 1;
}