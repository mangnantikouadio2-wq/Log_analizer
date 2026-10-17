#include <stdlib.h>
#include <ctype.h>
#include "parser.h"

enum {IN_BRACKETS=1, IN_QUOTES};

int parse_line(const char *line, LogEntity *entry) {
    LogEntity tmp = {0};
    int state = OUT;
    if (!isdigit((unsigned char)*line)) {
        return MISSING_IP;
    }
    get_ip(&line, tmp.ip);
    if (*line != ' ') {
        return 0;
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
            get_timestamp(&line, tmp.timestamp);
            if (*line != ' ') {
                return 0;
            }
            line++;
            if (*line != '+' && *line != '-') {
                return MISSING_TIMEZONE;
            }
            get_time_zone(&line, tmp.time_zone);
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
            get_method(&line, tmp.req.method);
            if (*line != ' ') {
                return 0;
            }
            line++;
            if (!get_link(&line, tmp.req.url, sizeof(tmp.req.url))) {
                return TOO_LONG_URL;
            }
            if (*line != ' ') {
                return 0;
            } else {
                line++;
            }
            if (!isalpha((unsigned char)*line)) {
                return MISSING_VERSION;
            }
            get_version(&line, tmp.req.http_version);
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
        return 0;
    }
    while(*line == ' ') {
        line++;
    }
    if (!isdigit((unsigned char)*line)) {
        return 0;
    }
    get_status(&line, tmp.status);
    if (*line != ' ') {
        return 0;
    }
    while(*line == ' ') {
        line++;
    }
    if (!isdigit((unsigned char)*line)) {
        return 0;
    }
    char *end;
    tmp.response_bytes = strtoul(line, &end, 10);
    if (end == line) {
        return MISSING_RESPONSE_SIZE;
    }
    line = end;
    if (*line != '\n' && *line != '\0') {
        return 0;
    }
    *entry = tmp;
    return 1;
}