#include "validator.h"
#include "statistics.h"

enum {is_weird, is_safe};

void display_top_N_ips(const Stats *s, size_t N) {
    printf("==== Top %ld IPs ====\n", N);
    for (size_t i = 0; i < N && i < s->IPs.n_ips; i++) {
        printf("%s : %lu requests\n", s->IPs.IpTable[i].ip, s->IPs.IpTable[i].count);
    }
}

void display_stats(const Stats *s) {
    printf("=== Statistiques ===\n");
    printf("Lignes lues       : %lu\n", s->total_lines);
    printf("Lignes valides    : %lu\n", s->parsed_ok);
    printf("Lignes rejetees   : %lu\n", s->rejected);
    printf("\n");
    printf("2xx (succès)      : %lu\n", s->successfull);
    printf("3xx (redirection) : %lu\n", s->redirect);
    printf("4xx (erreur cli)  : %lu\n", s->client_error);
    printf("5xx (erreur srv)  : %lu\n", s->server_error);
    printf("Other status code : %lu\n", s->other);
    printf("\n");
    printf("Top 10 IPs\n"); 
    for (size_t i = 0; i < 10 && i < s->IPs.n_ips; i++) {
        printf("%s : %lu requests\n", s->IPs.IpTable[i].ip, s->IPs.IpTable[i].count);
    }
    printf("\n");
    if (s->parsed_ok > 0) {
        printf("Taux de succès    : %.2f%%\n",
               100.0 * s->successfull / s->parsed_ok);
    }
}

void display_verbose_output(LogEntity current_entry, int line_count, int result) {
    printf("Line %d : ", line_count);
    int line_state = is_safe;
    if (result == 1) {
        if (validate_ip(current_entry.ip)) {
            printf("IP=%s | ", current_entry.ip);
        } else {
            printf("[INVALID IP: \"%s\"] | ", current_entry.ip);
            line_state = is_weird;
        }
        if (validate_timestamp(current_entry.timestamp)) {
            printf("TIMESTAMP=%s | ", current_entry.timestamp);
        } else {
            printf("[INVALID TIMESTAMP: \"%s\"] | ", current_entry.timestamp);
            line_state = is_weird;
        }
        if (validate_time_zone(current_entry.time_zone)) {
            printf("TIMEZONE=%s | ", current_entry.time_zone);
        } else {
            printf("[INVALID TIMEZONE: \"%s\"] | ", current_entry.time_zone);
            line_state = is_weird;
        }
        if (validate_method(current_entry.req.method)) {
            printf("METHOD=%s | ", current_entry.req.method);
        } else {
            printf("[INVALID METHOD: \"%s\"] | ", current_entry.req.method);
                line_state = is_weird;
        }
        if (validate_url(current_entry.req.url)) {
            printf("URL=%s | ", current_entry.req.url);
        } else {
            printf("[INVALID URL: \"%s\"] | ", current_entry.req.url);
            line_state = is_weird;
        }
        if (validate_version(current_entry.req.http_version)) {
            printf("HTTP_VERSION=%s | ", current_entry.req.http_version);
        } else {
            printf("[INVALID HTTP_VERSION: \"%s\"] | ", current_entry.req.http_version);
            line_state = is_weird;
        }
        if (validate_status(current_entry.status)) {
            printf("STATUS=%s | ", current_entry.status);
        } else {
            printf("[INVALID STATUS: \"%s\"] | ", current_entry.status);
            line_state = is_weird;
        }
        printf("RESPONSE_BYTES=%lu ", current_entry.response_bytes);
        switch (line_state) {
        case is_weird:
            printf("-> [WARNING]\n");
            break;
        
        default:
            printf("-> [OK]\n");
            break;
        }
    } else if (result == 0) {
        printf("INVALID LINE: BROKEN LINE STRUCTURE -> [REJECTED]!\n");
    } else if (result == MISSING_BRACKET) {
        printf("INVALID LINE: MISSING_BRACKET -> [REJECTED]!\n");
    } else if (result == MISSING_QUOTE) {
        printf("INVALID LINE: MISSING QUOTE -> [REJECTED]!\n");
    } else if (result == TOO_LONG_URL) {
        printf("INVALID LINE: TOO LONG URL -> [REJECTED]!\n");
    } else if (result == MISSING_IP) {
        printf("INVALID LINE: MISSING IP -> [REJECTED]!\n");
    } else if (result == MISSING_METHOD) {
        printf("INVALID LINE: MISSING METHOD -> [REJECTED]!\n");
    } else if (result == MISSING_RESPONSE_SIZE) {
        printf("INVALID LINE: MISSING RESPONSE SIZE -> [REJECTED]!\n");
    } else if (result == MISSING_TIMESTAMP) {
        printf("INVALID LINE: MISSING TIMESTAMP -> [REJECTED]!\n");
    } else if (result == MISSING_TIMEZONE) {
        printf("INVALID LINE: MISSING TIME ZONE -> [REJECTED]!\n");
    } else if (result == MISSING_VERSION) {
        printf("INVALID LINE: MISSING VERSION -> [REJECTED]!\n");
    }
}