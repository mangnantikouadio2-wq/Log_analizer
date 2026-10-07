#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"

enum {IN_BRACKETS=1, IN_QUOTES};

int parse_line(const char *line, LogEntity *entry) {
    int state = OUT;
    get_ip(&line, entry->ip);
    if (*line != ' ') {
        return MISSING_SPACE;
    }
    while (*line == ' ' || *line == '-') {
        line++;
    }
    if (*line++ == '[') {
        state = IN_BRACKETS;
        while (state == IN_BRACKETS) {
            get_timestamp(&line, entry->timestamp);
            if (*line != ' ') {
                return MISSING_SPACE;
            }
            line++;
            get_time_zone(&line, entry->time_zone);
            if (*line != ']') {
                return MISSING_BRACKET;
            }
            line++;
            state = OUT;
        }
    } else {
        return MISSING_BRACKET;
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
                return MISSING_SPACE;
            }
            line++;
            if (!get_link(&line, entry->req.url, sizeof(entry->req.url))) {
                return TOO_LONG_URL;
            }
            if (*line != ' ') {
                return MISSING_SPACE;
            } else {
                line++;
            }
            get_version(&line, entry->req.http_version);
            if (*line != '"') {
                return MISSING_QUOTE;
            }
            line++;
            state = OUT;
        }
    } else {
        return MISSING_QUOTE;
    }
    if (*line != ' ') {
        return MISSING_SPACE;
    }
    while(*line == ' ') {
        line++;
    }
    if (!isdigit((unsigned char)*line)) {
        return 0;
    }
    get_status(&line, entry->status);
    if (*line != ' ') {
        return MISSING_SPACE;
    }
    while(*line == ' ') {
        line++;
    }
    if (!isdigit((unsigned char)*line)) {
        return 0;
    }
    entry->response_bytes = get_size(line);
    return 1;
}