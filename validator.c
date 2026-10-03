#include <stdio.h>
#include <ctype.h>
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