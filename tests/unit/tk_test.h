#ifndef TK_TEST_H
#define TK_TEST_H

#include <stdio.h>
#include <string.h>

static int tk_checks;
static int tk_failures;

#define TK_CHECK(cond, ...) do {                                \
    tk_checks++;                                                \
    if (!(cond)) {                                              \
        tk_failures++;                                          \
        fprintf(stderr, "    FAIL %s:%d: ", __FILE__, __LINE__); \
        fprintf(stderr, __VA_ARGS__);                           \
        fprintf(stderr, "\n");                                  \
    }                                                           \
} while (0)

#define TK_EQ_STR(got, want) \
    TK_CHECK((got) != NULL && strcmp((got), (want)) == 0, \
             "expected \"%s\", got \"%s\"", (want), (got) ? (got) : "(null)")

#define TK_EQ_INT(got, want) \
    TK_CHECK((long)(got) == (long)(want), \
             "expected %ld, got %ld", (long)(want), (long)(got))

#define TK_RUN(fn) do { printf("  %s\n", #fn); fn(); } while (0)

static int tk_report(const char *suite) {
    printf("%s: %d checks, %d failed\n", suite, tk_checks, tk_failures);
    return tk_failures == 0 ? 0 : 1;
}

#endif
