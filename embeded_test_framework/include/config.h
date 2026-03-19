#ifndef CONFIG_H
#define CONFIG_H

typedef struct{
    int max_jobs;
    int timeout;
    int cpu_test_times;
    int memory_test_times;
    int disk_test_times;
    int device_test_times;
    int test_count;
    int cpu_test;
    int mem_test;
    int disk_test;
    int device_test;
    char report_file[128];
}Config;

typedef struct{
    char name[32];
    double duration;
    long memory_kb;
    int result;
}metrics_t;

int load_config(const char *filename,Config *cfg);

#endif