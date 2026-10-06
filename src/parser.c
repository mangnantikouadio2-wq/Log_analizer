#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"

int parse_line(const char *line, LogEntity *entry) {
    int state = OUT;
    get_ip(&line, entry->ip);
    while (*line == ' ' || *line == '-') {
        line++;
    }
    if (*line++ == '[') {
        state = IN_BRACKETS;
        while (state == IN_BRACKETS) {
            get_timestamp(&line, entry->timestamp);
            if (*line != ' ') {
                return 0;
            }
            line++;
            get_time_zone(&line, entry->time_zone);
            if (*line != ']') {
                return 0;
            }
            line++;
            state = OUT;
        }
    } else {
        return 0;
    }
    while (*line == ' ') {
        line++;
    }
    if (*line == '"') {
        state = IN_QUOTES;
        line++;
        while (state == IN_QUOTES) {
            get_method(&line, entry->req.method);
            if (*line != ' ') {
                return 0;
            }
            line++;
            get_link(&line, entry->req.url);
            if (*line != ' ') {
                return 0;
            } else {
                line++;
            }
            get_version(&line, entry->req.http_version);
            if (*line != '"') {
                return 0;
            }
            line++;
            state = OUT;
        }
    } else {
        return 0;
    }
    while(*line == ' ') {
        line++;
    }
    if (!isdigit((unsigned char)*line)) {
        return 0;
    }
    get_status(&line, entry->status);
    while(*line == ' ') {
        line++;
    }
    if (!isdigit((unsigned char)*line)) {
        return 0;
    }
    entry->response_bytes = get_size(line);
    return 1;
}