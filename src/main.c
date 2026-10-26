#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <ctype.h>
#include "statistics.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file> [--verbose] [--stats] [--top-ips N]\n", argv[0]);
        return 1;
    }
    char *file_name = argv[1];
    int do_verbose = 0;
    int do_stats = 0;
    int do_top = 0;
    int top_n = 10;
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "--verbose") == 0) {
            do_verbose = 1;
        } else if (strcmp(argv[i], "--stats") == 0) {
            do_stats = 1;
        } else if (strcmp(argv[i], "--top-ips") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Critical : expected a positive number!\n");
                return 1;
            }
            top_n = atoi(argv[++i]);
            if (top_n <= 0) {
                fprintf(stderr, "Error : the desired ip number must be positive\n");
                return 1;
            }
            do_top = 1;
        } else {
            fprintf(stderr, "Error : Unknown option '%s'\n", argv[i]);
            return 1;
        }
    }

    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        perror("Opening error");
        return 1;
    }
    Stats stats;
    stats_init(&stats);
    char line_buffer[BUFFER_SIZE];
    LogEntity current_entry;
    int line_count = 1;
    while (1) {
        char *line = get_line(fp, line_buffer, sizeof(line_buffer));
        if (line == NULL) {
            if (feof(fp)){ 
                break;
            }
            stats.total_lines++;
            if (do_verbose) {
                printf("Line %d : The line is too long -> [REJECTED]!\n", line_count);
            }
            line_count++;
            continue;
        }
        int result = parse_line(line_buffer, &current_entry);
        stats_record_parse_result(result, &stats);
        stats_update(&stats, &current_entry, result);
        if (do_verbose) {
            display_verbose_output(current_entry, line_count, result);
        }
        line_count++;
    }
    fclose(fp);
    if (do_stats) {
        qsort(stats.IPs.IpTable, stats.IPs.n_ips, sizeof(IP_count), compare_ips);
        display_stats(&stats);
    }
    if (do_top) {
        qsort(stats.IPs.IpTable, stats.IPs.n_ips, sizeof(IP_count), compare_ips);
        display_top_N_ips(&stats, top_n);
    }
    return 0;
}