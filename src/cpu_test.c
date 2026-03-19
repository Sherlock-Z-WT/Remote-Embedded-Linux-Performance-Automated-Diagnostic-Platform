#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/resource.h>
#include "tests.h"
#include "logger.h"

Metrics cpu_test(){
    Metrics metrics = {0};
    struct rusage start_usage, end_usage;
    clock_t start = clock();
    
    // 获取开始时的资源使用情况
    getrusage(RUSAGE_SELF, &start_usage);
    
    // CPU密集型任务
    volatile double x = 0;
    for(long i = 0; i < 100000000; i++){
        x += i * 0.000001;
    }
    
    // 获取结束时的资源使用情况
    getrusage(RUSAGE_SELF, &end_usage);
    clock_t end = clock();
    
    metrics.result = 0;
    metrics.execution_time = (double)(end - start) / CLOCKS_PER_SEC;
    
    // 计算CPU使用率（用户时间 + 系统时间）/ 总时间
    double user_time = (end_usage.ru_utime.tv_sec + end_usage.ru_utime.tv_usec / 1000000.0) - 
                      (start_usage.ru_utime.tv_sec + start_usage.ru_utime.tv_usec / 1000000.0);
    double sys_time = (end_usage.ru_stime.tv_sec + end_usage.ru_stime.tv_usec / 1000000.0) - 
                      (start_usage.ru_stime.tv_sec + start_usage.ru_stime.tv_usec / 1000000.0);
    double total_cpu_time = user_time + sys_time;
    metrics.cpu_usage = (int)((total_cpu_time / metrics.execution_time) * 100);
    if(metrics.cpu_usage > 100) metrics.cpu_usage = 100;
    
    // 内存使用量（单位：MB）
    metrics.memory_usage = (int)(end_usage.ru_maxrss / 1024.0);
    
    return metrics;
}