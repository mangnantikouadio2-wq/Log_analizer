#ifndef STATISTICS_H
#define STATISTICS_H

#include "parser.h"

#define MAX_STATUS 600

typedef struct {
    unsigned long total_lines;
    unsigned long parsed_ok;
    unsigned long rejected;
    unsigned long status_count[MAX_STATUS];
    unsigned long successfull;
    unsigned long redirect;
    unsigned long client_error;
    unsigned long server_error;
    unsigned long n_GET, n_POST, n_PUT, n_DELETE, n_HEAD,n_OPTIONS, n_PATCH, n_CONNECT, n_TRACE;
    unsigned long n_PROPFIND, n_MKCOL, n_COPY, n_MOVE, n_LOCK, n_UNLOCK, n_PLAY, n_PAUSE, n_other_method;
} Stats;

void stats_init(Stats *s);
void stats_record_parse_result(const int result, Stats *s);
void stats_update(Stats *s, const LogEntity *entry, int result);

#endif