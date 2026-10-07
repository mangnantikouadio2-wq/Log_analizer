#ifndef PARSER_H
#define PARSER_H

#define BUFFER_SIZE 512
#define OUT 0

enum error_cause {
    MISSING_SPACE = 2,
    MISSING_BRACKET,
    MISSING_QUOTE,
    TOO_LONG_URL
};

typedef struct {
char method[10];
char url[256];
char http_version[10];
} Request;

typedef struct {
char ip[16];
char timestamp[32];
char time_zone[8];
Request req;
char status[4];
unsigned long response_bytes;
} LogEntity;

char *get_line(FILE *fp, char *buffer, size_t size);
int get_status(const char **buffer,char *status);
int get_ip(const char **buffer, char *IP);
int get_timestamp(const char **buffer, char *timestamp);
int get_time_zone(const char **buffer, char *time_zone);
int get_method(const char **buffer, char *method);
int get_link(const char **buffer, char *URL, size_t capacity);
int get_version(const char **buffer, char *version);
int get_size(const char *buffer);
int parse_line(const char *line, LogEntity *entry);

#endif