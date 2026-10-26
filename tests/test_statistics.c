#include <stdio.h>
#include <string.h>
#include "statistics.h"

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

#define CHECK_ULONG(actual, expected) do {                                      \
    tests_run++;                                                                \
    if ((unsigned long)(actual) != (unsigned long)(expected)) {                 \
        tests_failed++;                                                         \
        fprintf(stderr, "  ❌ FAIL: %lu != %lu (line %d)\n",                    \
                (unsigned long)(actual), (unsigned long)(expected), __LINE__);  \
    }                                                                           \
} while (0)

static LogEntity make_entry(const char *ip, const char *method, 
                            const char *status, unsigned long size) {
    LogEntity e = {0};
    strncpy(e.ip, ip, sizeof(e.ip) - 1);
    strncpy(e.req.method, method, sizeof(e.req.method) - 1);
    strncpy(e.status, status, sizeof(e.status) - 1);
    e.response_bytes = size;
    return e;
}

void test_stats_init(void) {
    fprintf(stderr, "[stats_init]\n");
    Stats s;
    stats_init(&s);
    CHECK_ULONG(s.total_lines, 0);
    CHECK_ULONG(s.parsed_ok, 0);
    CHECK_ULONG(s.rejected, 0);
    CHECK_ULONG(s.successfull, 0);
    CHECK_ULONG(s.redirect, 0);
    CHECK_ULONG(s.client_error, 0);
    CHECK_ULONG(s.server_error, 0);
    CHECK_ULONG(s.n_GET, 0);
    CHECK_ULONG(s.IPs.n_ips, 0);
}

void test_stats_record_parse_result(void) {
    fprintf(stderr, "[stats_record_parse_result]\n");
    Stats s;
    stats_init(&s);
    stats_record_parse_result(1, &s);
    CHECK_ULONG(s.total_lines, 1);
    CHECK_ULONG(s.parsed_ok, 1);
    CHECK_ULONG(s.rejected, 0);
    stats_record_parse_result(0, &s);
    stats_record_parse_result(-1, &s);
    CHECK_ULONG(s.total_lines, 3);
    CHECK_ULONG(s.parsed_ok, 1);
    CHECK_ULONG(s.rejected, 2);
}

void test_classify_status(void) {
    fprintf(stderr, "[classify_status]\n");
    Stats s;
    stats_init(&s);
    /* Test 2xx */
    LogEntity e200 = make_entry("127.0.0.1", "GET", "200", 100);
    LogEntity e204 = make_entry("127.0.0.1", "GET", "204", 0);
    stats_update(&s, &e200, 1);
    stats_update(&s, &e204, 1);
    CHECK_ULONG(s.successfull, 2);
    /* Test 3xx */
    LogEntity e301 = make_entry("127.0.0.1", "GET", "301", 100);
    LogEntity e304 = make_entry("127.0.0.1", "GET", "304", 0);
    stats_update(&s, &e301, 1);
    stats_update(&s, &e304, 1);
    CHECK_ULONG(s.redirect, 2);
    /* Test 4xx */
    LogEntity e401 = make_entry("127.0.0.1", "GET", "401", 100);
    LogEntity e404 = make_entry("127.0.0.1", "GET", "404", 100);
    stats_update(&s, &e401, 1);
    stats_update(&s, &e404, 1);
    CHECK_ULONG(s.client_error, 2);
    /* Test 5xx */
    LogEntity e500 = make_entry("127.0.0.1", "GET", "500", 100);
    LogEntity e503 = make_entry("127.0.0.1", "GET", "503", 100);
    stats_update(&s, &e500, 1);
    stats_update(&s, &e503, 1);
    CHECK_ULONG(s.server_error, 2);
    /* Test hors plage (Other) */
    LogEntity e999 = make_entry("127.0.0.1", "GET", "999", 100);
    LogEntity e000 = make_entry("127.0.0.1", "GET", "000", 100);
    LogEntity e123 = make_entry("127.0.0.1", "GET", "123", 100);
    stats_update(&s, &e999, 1);
    stats_update(&s, &e000, 1);
    stats_update(&s, &e123, 1);
    CHECK_ULONG(s.other, 3);
}

void test_count_methods(void) {
    fprintf(stderr, "[count_methods]\n");
    Stats s;
    stats_init(&s);
    LogEntity e;
    e = make_entry("127.0.0.1", "GET", "200", 100);
    stats_update(&s, &e, 1);
    e = make_entry("127.0.0.1", "GET", "200", 100);
    stats_update(&s, &e, 1);
    e = make_entry("127.0.0.1", "POST", "200", 100);
    stats_update(&s, &e, 1);
    e = make_entry("127.0.0.1", "DELETE", "200", 100);
    stats_update(&s, &e, 1);
    e = make_entry("127.0.0.1", "HEAD", "200", 100);
    stats_update(&s, &e, 1);
    e = make_entry("127.0.0.1", "ALPHA", "200", 100);
    stats_update(&s, &e, 1);
    CHECK_ULONG(s.n_GET, 2);
    CHECK_ULONG(s.n_POST, 1);
    CHECK_ULONG(s.n_DELETE, 1);
    CHECK_ULONG(s.n_HEAD, 1);
    CHECK_ULONG(s.n_PUT, 0);
    CHECK_ULONG(s.n_other_method, 1);
}

void test_count_ips(void) {
    fprintf(stderr, "[count_ips / IPTable]\n");
    Stats s;
    stats_init(&s);
    LogEntity e;
    e = make_entry("192.168.1.50", "GET", "200", 100);
    stats_update(&s, &e, 1);
    stats_update(&s, &e, 1);
    stats_update(&s, &e, 1);
    e = make_entry("10.0.0.12", "GET", "200", 100);
    stats_update(&s, &e, 1);
    e = make_entry("127.0.0.1", "GET", "200", 100);
    stats_update(&s, &e, 1);
    stats_update(&s, &e, 1);
    CHECK_ULONG(s.IPs.n_ips, 3);
    int found = 0;
    for (size_t i = 0; i < s.IPs.n_ips; i++) {
        if (strcmp(s.IPs.IpTable->ip, "192.168.1.50") == 0) {
            CHECK_ULONG(s.IPs.IpTable[i].count, 3);
            found = 1;
            break;
        }
    }
    CHECK(found == 1);
    found = 0;
    for (size_t i = 0; i < s.IPs.n_ips; i++) {
        if (strcmp(s.IPs.IpTable[i].ip, "10.0.0.12") == 0) {
            CHECK_ULONG(s.IPs.IpTable[i].count, 1);
            found = 1;
            break;
        }
    }
    CHECK(found == 1);
}

int main(void) {
    test_stats_init();
    test_stats_record_parse_result();
    test_classify_status();
    test_count_methods();
    test_count_ips();
    
    fprintf(stderr, "\n=== %d tests, %d échecs ===\n", tests_run, tests_failed);
    return tests_failed > 0;
}