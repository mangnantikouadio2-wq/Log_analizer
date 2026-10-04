#include <stdio.h>
#include <string.h>
#include "validator.h"

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

void test_validate_ip(void) {
    fprintf(stderr, "[validate_ip]\n");
    CHECK(validate_ip("192.168.1.50") == 1);
    CHECK(validate_ip("0.0.0.0")      == 1);
    CHECK(validate_ip("255.255.255.255") == 1);
    CHECK(validate_ip("256.1.1.1")    == 0);
    CHECK(validate_ip("1.2.3")        == 0);
    CHECK(validate_ip("1.2.3.4.5")    == 0);
    CHECK(validate_ip("")             == 0);
    CHECK(validate_ip("abc")          == 0);
}

void test_validate_status(void) {
    fprintf(stderr, "[validate_status]\n");
    CHECK(validate_status("200") == 1);
    CHECK(validate_status("404") == 1);
    CHECK(validate_status("599") == 1);
    CHECK(validate_status("099") == 0);
    CHECK(validate_status("600") == 0);
    CHECK(validate_status("4012") == 0);
}

void test_validate_timestamp(void) {
    fprintf(stderr, "[validate_timestamp]\n");
    CHECK(validate_timestamp("24/Aug/2026:10:15:30") == 1);
    CHECK(validate_timestamp("01/Jan/2000:00:00:00") == 1);
    CHECK(validate_timestamp("23/Dec/2099:23:59:59") == 1);
    CHECK(validate_timestamp("09/Sep/2009:09:09:09") == 1);
    CHECK(validate_timestamp("00/Aug/2026:10:15:30") == 0);
    CHECK(validate_timestamp("32/Aug/2026:10:15:30") == 0);  
    CHECK(validate_timestamp("99/Aug/2026:10:15:30") == 0);  
    CHECK(validate_timestamp("0a/Aug/2026:10:15:30") == 0);  
    CHECK(validate_timestamp("2/Aug/2026:10:15:30")  == 0);  
    CHECK(validate_timestamp(" 1/Aug/2026:10:15:30") == 0);
    CHECK(validate_timestamp("24/Xxx/2026:10:15:30") == 0);  
    CHECK(validate_timestamp("24/aug/2026:10:15:30") == 0);  
    CHECK(validate_timestamp("24/AUG/2026:10:15:30") == 0);  
    CHECK(validate_timestamp("24/Au/2026:10:15:30")  == 0);  
    CHECK(validate_timestamp("24/Augu/2026:10:15:30")== 0); 
    CHECK(validate_timestamp("24/Aug2026:10:15:30")  == 0);  
    CHECK(validate_timestamp("24/Aug/999:10:15:30")  == 0);  
    CHECK(validate_timestamp("24/Aug/20266:10:15:30")== 0); 
    CHECK(validate_timestamp("24/Aug/20a6:10:15:30") == 0);  
    CHECK(validate_timestamp("24/Aug/2026-10:15:30") == 0);  
    CHECK(validate_timestamp("24/Aug/2026:24:15:30") == 0);  
    CHECK(validate_timestamp("24/Aug/2026:10:60:30") == 0);  
    CHECK(validate_timestamp("24/Aug/2026:10:15:60") == 0);  
    CHECK(validate_timestamp("24/Aug/2026:10:15")    == 0);  
    CHECK(validate_timestamp("24/Aug/2026:1:15:30")  == 0);  
    CHECK(validate_timestamp("24/Aug/2026:10-15-30") == 0);  
    CHECK(validate_timestamp("24/Aug/2026:10:15:3a") == 0);  
    CHECK(validate_timestamp("")                     == 0);   
    CHECK(validate_timestamp("24")                   == 0);   
    CHECK(validate_timestamp("24/Aug/2026:10:15:30extra")== 0);
}

void test_validate_method(void) {
    fprintf(stderr, "[validate_method]\n");
    CHECK(validate_method("GET")     == 1);
    CHECK(validate_method("POST")    == 1);
    CHECK(validate_method("PUT")     == 1);
    CHECK(validate_method("DELETE")  == 1);
    CHECK(validate_method("PATCH")   == 1);
    CHECK(validate_method("HEAD")    == 1);
    CHECK(validate_method("OPTIONS") == 1);
    CHECK(validate_method("")        == 0);  
    CHECK(validate_method("get")     == 0);  
    CHECK(validate_method("Get")     == 0);  
    CHECK(validate_method("G E T")   == 0);  
    CHECK(validate_method("FOO")     == 0);  
    CHECK(validate_method("GETPOST") == 0);  
    CHECK(validate_method("G3T")     == 0);  
    CHECK(validate_method("G@T")     == 0);  
    CHECK(validate_method("CONNECT") == 1);  
    CHECK(validate_method("TRACE")   == 1);
}

void test_validate_url(void) {
    fprintf(stderr, "[validate_url]\n");
    CHECK(validate_url("/")                    == 1);  
    CHECK(validate_url("/index.html")          == 1);
    CHECK(validate_url("/admin/login")         == 1);
    CHECK(validate_url("/api/users?id=42")     == 1);  
    CHECK(validate_url("/static/css/style.css")== 1);
    CHECK(validate_url("/a")                   == 1);  
    CHECK(validate_url("")            == 0);  
    CHECK(validate_url("index.html")  == 0);  
    CHECK(validate_url(" /index.html")== 0); 
    CHECK(validate_url("/index html") == 0);  
    CHECK(validate_url("/index\n")    == 0);  
    CHECK(validate_url("/index\t")    == 0);  
    CHECK(validate_url("/index\"")    == 0);  
    CHECK(validate_url("/index\x01")  == 0);  
    CHECK(validate_url("/index\x1F")  == 0);  
    CHECK(validate_url("/index\x7F")  == 0);
}

int main(void) {
    test_validate_ip();
    test_validate_status();
    test_validate_timestamp();
    test_validate_method();
    test_validate_url();
    fprintf(stderr, "\n=== %d tests, %d échecs ===\n",
            tests_run, tests_failed);
    return tests_failed > 0;
}