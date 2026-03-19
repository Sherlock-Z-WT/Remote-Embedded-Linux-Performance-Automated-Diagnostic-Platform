#include "test_registry.h"
#include "tests.h"

test_case_t test_registry[] = {
    {"cpu_test",cpu_test},
    {"mem_test",mem_test},
    {"disk_test",disk_test},
    {"device_test",device_test},
};

int test_count = sizeof(test_registry) / sizeof(test_case_t);