#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int parse_line(const char *line, LogEntity *entry) {
    int state = OUT;
    fprintf(stderr, "[p] entrée: <<%s>>\n", line);
    get_ip(&line, entry->ip);
    fprintf(stderr, "[p] après get_ip: ip='%s' | reste='%.30s'\n", entry->ip, line);
    while (*line == ' ' || *line == '-') {
        line++;
    }
    fprintf(stderr, "[p] après skip: '%c' (0x%02x)\n", *line ? *line : '?', (unsigned char)*line);
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
        fprintf(stderr, "[p] ECHEC: pas de '['\n");
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
        fprintf(stderr, "[p] ECHEC: pas de '\"'\n");
        return 0;
    }
    while(*line && (*line < '0' || *line > '9')) {
        line++;
    }
    entry->status = get_status(line);
    return 1;
}