#include <stdio.h>
#include <stdlib.h>
#include "statistics.h"


static void is_code_in_range(const int code, unsigned long *count, int begin, int end) {
    if (code >= begin && code < end) {
        (*count)++;
    }
}

void stats_record_parse_result(const int result, Stats *s) {
    s->total_lines++;
    if (result == 1) {
        s->parsed_ok++;
    } else {
        s->rejected++;
    }
}

static void classyfy_status(const char *status, Stats *s) {
    int code = atoi(status);
    is_code_in_range(code, &s->successfull, 200, 300);
    is_code_in_range(code, &s->redirect, 300, 400);
    is_code_in_range(code, &s->client_error, 400, 500);
    is_code_in_range(code, &s->server_error, 500, 600);   
}

void stats_init(Stats *s) {
    *s = (Stats){0};
}

void stats_update(Stats *s, const LogEntity *entry) {
    classyfy_status(entry->status, s);
}