#include <stdio.h>
#include <time.h>
#include "tests.h"
#include "logger.h"
#include "device.h"

Metrics device_test(){
    Metrics metrics = {0};
    clock_t start = clock();
    
    device_init("192.168.1.1",22,"admin","password");
    
    if(device_connect() != 0){
        metrics.result = 1;
        metrics.execution_time = (double)(clock() - start) / CLOCKS_PER_SEC;
        return metrics;
    }
    
    DeviceMetrics device_metrics;
    if(device_get_metrics(&device_metrics) != 0){
        metrics.result = 1;
    } else {
        metrics.result = 0;
        metrics.cpu_usage = (int)device_metrics.cpu_usage;
        
        if(device_metrics.memory_total > 0){
            metrics.memory_usage = (device_metrics.memory_used * 100) / device_metrics.memory_total;
        }
        
        if(device_metrics.disk_total > 0){
            metrics.disk_usage = (device_metrics.disk_used * 100) / device_metrics.disk_total;
        }
    }
    
    device_disconnect();
    
    metrics.execution_time = (double)(clock() - start) / CLOCKS_PER_SEC;
    return metrics;
}
