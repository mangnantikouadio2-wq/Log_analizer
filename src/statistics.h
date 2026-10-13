#ifndef STATISTICS_H
#define STATISTICS_H

#include "parser.h"

typedef struct {
    unsigned long total_lines;
    unsigned long parsed_ok;
    unsigned long rejected;
    unsigned long successfull;
    unsigned long redirect;
    unsigned long client_error;
    unsigned long server_error;
} Stats;

void stats_init(Stats *s);
void stats_record_parse_result(const int result, Stats *s);
void stats_update(Stats *s, const LogEntity *entry);

#endif