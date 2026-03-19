#include <stdio.h>
#include <time.h>
#include <sys/resource.h>
#include "tests.h"
#include "logger.h"

Metrics disk_test(){
    Metrics metrics = {0};
    struct rusage start_usage, end_usage;
    clock_t start = clock();
    
    // 获取开始时的资源使用情况
    getrusage(RUSAGE_SELF, &start_usage);
    
    FILE *fp = fopen("testfile.tmp","w+");
    if(!fp){
        metrics.result = 1;
        metrics.execution_time = 0;
        return metrics;
    }
    
    int io_ops = 0;
    char buffer[1024] = {0};
    
    // 写入测试
    for(int i = 0; i < 10000; i++){
        snprintf(buffer, sizeof(buffer), "disk test line %d\n", i);
        fwrite(buffer, 1, strlen(buffer), fp);
        io_ops++;
    }
    
    // 刷新缓冲区，确保数据写入磁盘
    fflush(fp);
    fsync(fileno(fp));
    
    // 读取测试
    fseek(fp, 0, SEEK_SET);
    while(fread(buffer, 1, sizeof(buffer), fp) > 0){
        io_ops++;
    }
    
    fclose(fp);
    
    // 获取结束时的资源使用情况
    getrusage(RUSAGE_SELF, &end_usage);
    clock_t end = clock();
    
    metrics.result = 0;
    metrics.execution_time = (double)(end - start) / CLOCKS_PER_SEC;
    metrics.io_ops = io_ops;
    
    // 计算CPU使用率
    double user_time = (end_usage.ru_utime.tv_sec + end_usage.ru_utime.tv_usec / 1000000.0) - 
                      (start_usage.ru_utime.tv_sec + start_usage.ru_utime.tv_usec / 1000000.0);
    double sys_time = (end_usage.ru_stime.tv_sec + end_usage.ru_stime.tv_usec / 1000000.0) - 
                      (start_usage.ru_stime.tv_sec + start_usage.ru_stime.tv_usec / 1000000.0);
    double total_cpu_time = user_time + sys_time;
    metrics.cpu_usage = (int)((total_cpu_time / metrics.execution_time) * 100);
    if(metrics.cpu_usage > 100) metrics.cpu_usage = 100;
    
    // 内存使用量（单位：MB）
    metrics.memory_usage = (int)(end_usage.ru_maxrss / 1024.0);
    
    // 磁盘使用量（单位：MB）
    metrics.disk_usage = 1; // 实际文件大小约为100KB，这里简化处理
    
    return metrics;
}