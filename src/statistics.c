#include <stdlib.h>
#include <string.h>
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
    is_code_in_range(code, &s->status_count[code], 100, MAX_STATUS);
    is_code_in_range(code, &s->successfull, 200, 300);
    is_code_in_range(code, &s->redirect, 300, 400);
    is_code_in_range(code, &s->client_error, 400, 500);
    is_code_in_range(code, &s->server_error, 500, 600);   
}

static void count_methods(const char *method, Stats *s) {
    if (strcmp(method, "GET") == 0) s->n_GET++;
    if (strcmp(method, "POST")== 0) s->n_POST++;
    if (strcmp(method, "PUT") == 0) s->n_PUT++;
    if (strcmp(method, "DELETE")== 0) s->n_DELETE++;
    if (strcmp(method, "HEAD")== 0) s->n_HEAD++;
    if (strcmp(method, "OPTIONS")== 0) s->n_OPTIONS++;
    if (strcmp(method, "PATCH")== 0) s->n_PATCH++;
    if (strcmp(method, "CONNECT")== 0) s->n_CONNECT++;
    if (strcmp(method, "TRACE")== 0) s->n_TRACE++;
    if (strcmp(method, "PROPFIND")== 0) s->n_PROPFIND++;
    if (strcmp(method, "MKCOL") == 0) s->n_MKCOL++;
    if (strcmp(method, "COPY")  == 0) s->n_COPY++;
    if (strcmp(method, "MOVE")  == 0) s->n_MOVE++;
    if (strcmp(method, "LOCK")  == 0) s->n_LOCK++;
    if (strcmp(method, "UNLOCK")== 0) s->n_UNLOCK++;
    if (strcmp(method, "PLAY")  == 0) s->n_PLAY++;
    if (strcmp(method, "PAUSE") == 0) s->n_PAUSE++;
    else s->n_other_method++;
}

void stats_init(Stats *s) {
    *s = (Stats){0};
}

void stats_update(Stats *s, const LogEntity *entry, int result) {
    if (result == 1) {
        count_methods(entry->req.method, s);
        classyfy_status(entry->status, s);
    }
}