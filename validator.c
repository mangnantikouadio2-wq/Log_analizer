#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "parser.h"

int validate_ip(char *ip) {
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

int validate_timestamp(char *timestamp) {
    
}

static int is_valid_day(const char *day) {
    if (!isdigit((unsigned char)day[0]) || !isdigit((unsigned char)day[1])) {
        return 0;
    }
    if (day[2] != '/') {
        return 0;
    }
    int d = (day[0] - '0') * 10 + (day[1] - '0');
    return d>=1 && d<=31;
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