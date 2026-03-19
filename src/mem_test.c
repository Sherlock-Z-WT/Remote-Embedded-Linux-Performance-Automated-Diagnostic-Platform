#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include "tests.h"
#include "logger.h"

Metrics mem_test(){
    Metrics metrics = {0};
    struct rusage start_usage, end_usage;
    clock_t start = clock();
    
    // 获取开始时的资源使用情况
    getrusage(RUSAGE_SELF, &start_usage);
    
    // 分配较大的内存块
    int size = 100 * 1024 * 1024; // 100MB
    char *p = malloc(size);
    
    // 获取结束时的资源使用情况
    getrusage(RUSAGE_SELF, &end_usage);
    clock_t end = clock();
    
    if(p != NULL){
        // 填充内存以确保实际分配
        memset(p, 0, size);
        
        // 计算内存使用量（单位：MB）
        metrics.memory_usage = (int)(end_usage.ru_maxrss / 1024.0);
        
        free(p);
        metrics.result = 0;
    } else {
        metrics.result = 1;
        metrics.memory_usage = 0;
    }
    
    metrics.execution_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    // 计算CPU使用率
    double user_time = (end_usage.ru_utime.tv_sec + end_usage.ru_utime.tv_usec / 1000000.0) - 
                      (start_usage.ru_utime.tv_sec + start_usage.ru_utime.tv_usec / 1000000.0);
    double sys_time = (end_usage.ru_stime.tv_sec + end_usage.ru_stime.tv_usec / 1000000.0) - 
                      (start_usage.ru_stime.tv_sec + start_usage.ru_stime.tv_usec / 1000000.0);
    double total_cpu_time = user_time + sys_time;
    metrics.cpu_usage = (int)((total_cpu_time / metrics.execution_time) * 100);
    if(metrics.cpu_usage > 100) metrics.cpu_usage = 100;
    
    return metrics;
}
