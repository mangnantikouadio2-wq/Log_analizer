#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"

enum {IN_BRACKETS=1, IN_QUOTES};

int parse_line(const char *line, LogEntity *entry) {
    int state = OUT;
    if (!isdigit((unsigned char)*line)) {
        return MISSING_IP;
    }
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
            if (!isdigit((unsigned char)*line)) {
                return MISSING_TIMESTAMP;
            }
            get_timestamp(&line, entry->timestamp);
            if (*line != ' ') {
                return MISSING_SPACE;
            }
            line++;
            if (*line != '+' && *line != '-') {
                return MISSING_TIMEZONE;
            }
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
            if (!isalpha((unsigned char)*line)) {
                return MISSING_METHOD;
            }
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
            if (!isalpha((unsigned char)*line)) {
                return MISSING_VERSION;
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
    char *end;
    entry->response_bytes = strtoul(line, &end, 10);
    if (end == line) {
        return MISSING_RESPONSE_SIZE;
    }
    line = end;
    if (*line != '\n' && *line != '\0') {
        return 0;
    }
    return 1;
}