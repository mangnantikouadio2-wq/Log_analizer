#include <stdio.h>
#include "parser.h"
#include "validator.h"

enum {is_weird, is_safe};

int main(int argc, char *argv[]) {
    char *file_name = (argc > 1) ? argv[1] : "log.txt";
    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        printf("OPENING ERROR\n");
        return 1;
    }
    int line_count = 1;
    int line_state;
    char line_buffer[BUFFER_SIZE];
    LogEntity current_entry;
    while(1) {
        char *line = get_line(fp, line_buffer, sizeof(line_buffer));
        if (line == NULL) {
            if (feof(fp)) {
                break;
            }
            printf("Line %d : The line is too long -> [REJECTED]!\n", line_count);
            continue;
        }
        line_state = is_safe;
        printf("Line %d : ", line_count);
        int result = parse_line(line_buffer, &current_entry);
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
        line_count++;
    }
    fclose(fp);
    return 0;
}