#ifndef METRICS_H
#define METRICS_H

typedef struct{
    int result;          // 测试结果：0表示成功，非0表示失败
    double execution_time; // 执行时间（秒）
    int cpu_usage;       // CPU使用率（百分比）
    int memory_usage;    // 内存使用量（MB）
    int disk_usage;      // 磁盘使用量（MB）
    int io_ops;          // I/O操作次数
}Metrics;

#endif