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
    if (code < 200 || code > 600) {
        s->other++;
    }
}

static void count_methods(const char *method, Stats *s) {
    if (strcmp(method, "GET") == 0) s->n_GET++;
    else if (strcmp(method, "POST")== 0) s->n_POST++;
    else if (strcmp(method, "PUT") == 0) s->n_PUT++;
    else if (strcmp(method, "DELETE") == 0) s->n_DELETE++;
    else if (strcmp(method, "HEAD")   == 0) s->n_HEAD++;
    else if (strcmp(method, "OPTIONS")== 0) s->n_OPTIONS++;
    else if (strcmp(method, "PATCH")  == 0) s->n_PATCH++;
    else if (strcmp(method, "CONNECT")== 0) s->n_CONNECT++;
    else if (strcmp(method, "TRACE")  == 0) s->n_TRACE++;
    else if (strcmp(method, "PROPFIND")== 0) s->n_PROPFIND++;
    else if (strcmp(method, "MKCOL") == 0) s->n_MKCOL++;
    else if (strcmp(method, "COPY")  == 0) s->n_COPY++;
    else if (strcmp(method, "MOVE")  == 0) s->n_MOVE++;
    else if (strcmp(method, "LOCK")  == 0) s->n_LOCK++;
    else if (strcmp(method, "UNLOCK")== 0) s->n_UNLOCK++;
    else if (strcmp(method, "PLAY")  == 0) s->n_PLAY++;
    else if (strcmp(method, "PAUSE") == 0) s->n_PAUSE++;
    else s->n_other_method++;
}

static void ip_increment(IP_Table *t, const char *ip) {
    for (size_t i = 0; i < t->n_ips; i++) {
        if (strcmp(t->IpTable[i].ip, ip) == 0) {
            t->IpTable[i].count++;
            return;
        }
    }
    if (t->n_ips < MAX_IPs) {
        strcpy(t->IpTable[t->n_ips].ip, ip);
        t->IpTable[t->n_ips].count = 1;
        t->n_ips++;
    }
}

int compare_ips(const void *ip_a, const void *ip_b) {
    const IP_count *count_a = (const IP_count *)ip_a;
    const IP_count *count_b = (const IP_count *)ip_b;
    if (count_a->count < count_b->count) {
        return 1;
    }
    if (count_a->count > count_b->count) {
        return -1;
    }
    return 0;
}

void stats_init(Stats *s) {
    *s = (Stats){0};
}

void stats_update(Stats *s, const LogEntity *entry, int result) {
    if (result == 1) {
        ip_increment(&s->IPs, entry->ip);
        count_methods(entry->req.method, s);
        classyfy_status(entry->status, s);
    }
}