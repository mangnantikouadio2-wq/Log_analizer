#ifndef PARSER_H
#define PARSER_H

#define BUFFER_SIZE 512
#define OUT 0

enum {IN_BRACKETS=1, IN_QUOTES};

typedef struct {
char method[10];
char url[256];
} Request;

typedef struct {
char ip[16];
char timestamp[32];
Request req;
int status;
} LogEntity;

char *get_line(FILE *fp, char *buffer, size_t size);
int get_status(const char *buffer);
int get_ip(const char **buffer, char *IP);
int get_timestamp(const char **buffer, char *timestamp);
int get_method(const char **buffer, char *method);
int get_link(const char **buffer, char *URL);
int parse_line(const char *line, LogEntity *entry);

#endif