#include <stdio.h>
#include <string.h>
#include "parser.h"

static int tests_run = 0;
static int tests_failed = 0;

#define CHECK(cond) do {                                            \
    tests_run++;                                                    \
    if (!(cond)) {                                                  \
        tests_failed++;                                             \
        fprintf(stderr, "  ❌FAIL %s (ligne %d)\n",                 \
                #cond, __LINE__);                                   \
    } else {                                                        \
        fprintf(stderr, "  ✅ %s\n", #cond);                        \
    }                                                               \
} while (0)

void test_parse_line_for_valid_line(void) {
    fprintf(stderr, "[parse_line]: valid line\n");
    LogEntity e;
    char line[] = "192.168.1.50 - - [24/Aug/2026:10:15:30 +0000] \"GET /admin/login HTTP/1.1\" 401 512\n";
    int r = parse_line(line, &e);
    CHECK(r == 1);
    CHECK(strcmp(e.ip, "192.168.1.50") == 0);
    CHECK(strcmp(e.timestamp, "24/Aug/2026:10:15:30") == 0);
    CHECK(strcmp(e.time_zone, "+0000") == 0);
    CHECK(strcmp(e.req.method, "GET") == 0);
    CHECK(strcmp(e.req.url, "/admin/login") == 0);
    CHECK(strcmp(e.req.http_version, "HTTP/1.1") == 0);
    CHECK(strcmp(e.status, "401") == 0);
    CHECK(e.response_bytes == 512);
}

void test_parse_line_for_empty_line(void) {
    fprintf(stderr, "[parse_line]: empty line\n");
    LogEntity e;
    CHECK(parse_line("", &e) == MISSING_SPACE);
}

void test_parse_line_for_missing_patterns(void) {
    fprintf(stderr, "[parse_line]: missing brackets\n");
    LogEntity e;
    char line[] = "192.168.1.50 - - 24/Aug/2026:10:15:30 \"GET / HTTP/1.1\" 200\n";
    CHECK(parse_line(line, &e) == MISSING_BRACKET);
}

void test_parse_line_for_malformed_lines(void) {
    fprintf(stderr, "[parse_line]: malformed lines\n");
    LogEntity e;
    char line1[] = "192.168.1.6 - - [10/Oct/2026:10:00: 6 +0000] GET /malformed_line1 HTTP/1.1 500 5678";
    CHECK(parse_line(line1, &e) == 0);
    char line2[] = "- - [10/Oct/2026:10:00:06 +0000] GET /malformed_line2 HTTP/1.1 500 5678";
    CHECK(parse_line(line2, &e) == 0);
    char line3[] = "192.168.1.6 - - [10/Oct/2026:10:00:06 +0000] \"GET /malformed_line3 HTTP/1.1\" 500 5678 extra_field";
    CHECK(parse_line(line3, &e) == 0);
}

void test_get_method_for_edge_case(void) {
    fprintf(stderr, "[get_method]: buffer overflow\n");
    char big[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    const char *p = big;
    char method[10];
    get_method(&p, method);
    CHECK(strlen(method) <= 9);
    CHECK(method[9] == '\0');
}

void test_get_link_for_edge_case1(void) {
    fprintf(stderr, "[get_link]: buffer overflow\n");
    char big[600];
    memset(big, 'a', 599);
    big[0] = '/';
    big[599] = '\0';
    char url[256];
    const char *p = big;
    get_link(&p, url, sizeof(url));
    CHECK(strlen(url) <= 255);
    CHECK(url[255] == '\0');
}

void test_get_link_for_edge_case2(void) {
    fprintf(stderr, "[get_link]: no closing quote");
    char no_quote[] = "/an/endless/url/that/continue/again/and/again";
    const char *p = no_quote;
    char url[256];
    get_link(&p, url, sizeof(url));
    CHECK(strlen(url) <256);
}

int main(void) {
    test_parse_line_for_valid_line();
    test_parse_line_for_missing_patterns();
    test_parse_line_for_malformed_lines();
    test_parse_line_for_empty_line();
    test_get_method_for_edge_case();
    test_get_link_for_edge_case1();
    test_get_link_for_edge_case2();
    fprintf(stderr, "\n=== %d tests, %d échecs ===\n",
            tests_run, tests_failed);
    return tests_failed > 0;
}