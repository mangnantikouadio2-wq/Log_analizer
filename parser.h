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
int parse_line(const char *line, LogEntity *entry);

#endif