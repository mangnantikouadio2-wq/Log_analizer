#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"

int validate_ip(const char *ip) {
   int octets = 0;
   int value = 0;
   int digits = 0;
   while(*ip) {
    if (isdigit((unsigned char)*ip)) {
        value = value * 10 + (*ip - '0');
        digits++;
        if (value > 255) {
            return 0;
        }
        if (digits > 3) {
            return 0;
        }
    } else if (*ip == '.') {
        if (digits == 0) {
            return 0;
        }
        octets++;
        value =0;
        digits = 0;
    } else {
        return 0;
    }
    ip++;
   }
   if (digits == 0) {
    return 0;
   }
   octets++;
   return (octets == 4);
}

int validate_timestamp(const char *timestamp) {
    if (!is_valid_day(timestamp)) {
        return 0;
    }
    if (!is_valid_month(timestamp + 3)) {
        return 0;
    }
    if (!is_valid_year(timestamp + 7)) {
        return 0;
    }
    if (!is_valid_time(timestamp + 12)) {
        return 0;
    }
    return 1;
}

static int is_valid_day(const char *day) {
    if (!is_two_digits(day, 1, 31)) {
        return 0;
    }
    if (day[2] != '/') {
        return 0;
    }
    return 1;
}

static int is_valid_month(const char *month) {
    static char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul",
                            "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i=0; i < 12; i++) {
        if (strncmp(month, months[i], 3) == 0) {
            return 1;
        }
    }
    return 0;
}

static int is_valid_year(const char *year) {
    for (int i=0; i < 4; i++) {
        if (!isdigit((unsigned char)year[i])) {
            return 0;
        }
    }
    if (year[4] != ':') {
        return 0;
    }
    return 1;
}

static int is_two_digits(const char *s, int min, int max) {
    if (!isdigit((unsigned char)s[0]) || !isdigit((unsigned char)s[1])) {
        return 0;
    };
    int d = (s[0] - '0') * 10 + (s[0] - '0');
    return d>=min && d<=max;
}

static int is_valid_time(const char *time) {
    if (!is_two_digits(time, 0, 23)) {
        return 0;
    }
    if (time[2] != ':') {
        return 0;
    }
    if (!is_two_digits(time + 3, 0, 59)) {
        return 0;
    }
    if (time[5] != ':') {
        return 0;
    }
    if (!is_two_digits(time + 6, 0, 59)) {
        return 0;
    }
    return 1;
}

int validate_method(const char *method) {
    for (int c=0; method[c] != '\0'; c++) {
        if (!isalpha((unsigned char)method[c])) {
            return 0;
        }
    }
    return 1;
}

int validate_url(const char *link) {
    if (link[0] != '/') {
        return 0;
    }
    for (const char *p = link; *p; p++) {
        unsigned char c = (unsigned char)*p;
        if (c <= 0x20 || c == 0x7F) {
            return 0;
        }
        if (c == '"') {
            return 0;
        }
        if (c == '<' || c == '>' || c == '\\') {
            return 0;
        }
    }
    return 1;
}