#include <stdio.h>
#include "parser.h"
#include "validator.h"

enum {is_weird, is_safe};

int main(void) {
    FILE *fp = fopen("log.txt", "r");
    if (!fp) {
        printf("OPENING ERROR\n");
        return 1;
    }
    int line_count = 1;
    int line_state;
    char line_buffer[BUFFER_SIZE];
    LogEntity current_entry;
    while(fgets(line_buffer, sizeof(line_buffer), fp) != NULL) {
        line_state = is_safe;
        if (parse_line(line_buffer, &current_entry)) {
            printf("Line %d : ", line_count);
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
            if (validate_status(current_entry.status)) {
                printf("STATUS=%s ", current_entry.status);
            } else {
                printf("[INVALID STATUS: \"%s\"] ", current_entry.status);
                line_state = is_weird;
            }
            switch (line_state) {
            case is_weird:
                printf("-> [WARNING]\n");
                break;
            
            default:
                printf("-> [OK]\n");
                break;
            }
        } else {
            printf("INVALID LINE(corrupted structure) -> [REJECTED]!\n");
        }
        line_count++;
    }
    fclose(fp);
    return 0;
}