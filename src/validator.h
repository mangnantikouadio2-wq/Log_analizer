#ifndef VALIDATOR_H
#define VALIDATOR_H

int validate_ip(const char *ip);
int validate_timestamp(const char *timestamp);
int validate_time_zone(const char *zone);
int validate_method(const char *method);
int validate_url(const char *link);
int validate_status(const char *status);
int validate_version(const char *version);
int is_valid_size(const char *size);

#endif