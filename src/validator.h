#ifndef PARSER_H
#define PARSER_H

int validate_ip(const char *ip);
int validate_timestamp(const char *timestamp);
int validate_method(const char *method);
int validate_url(const char *link);
int validate_status(const char *status);

#endif