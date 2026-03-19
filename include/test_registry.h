#ifndef TEST_REGISTRY_H
#define TEST_REGISTRY_H

#include "metrics.h"

typedef struct{
    const char *name;
    Metrics (*func)();
}test_case_t;

extern test_case_t test_registry[];
extern int test_count;

#endif
