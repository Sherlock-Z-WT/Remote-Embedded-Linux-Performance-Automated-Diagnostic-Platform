#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include "test_runner.h"
#include "logger.h"
#include "report.h"
#include "tests.h"
#include "test_registry.h"
#include "metrics.h"


void run_tests(Config *cfg,int max_jobs,int timeout){
    report_init();
    report_init_json();
    
    // 确保max_jobs至少为1
    if(max_jobs < 1){
        max_jobs = 1;
    }
    
    log_info("Starting test runner with max_jobs=%d, timeout=%d\n", max_jobs, timeout);
    log_debug("Debug mode: test_count=%d, cpu_test=%d, mem_test=%d, disk_test=%d\n", 
             cfg->test_count, cfg->cpu_test, cfg->mem_test, cfg->disk_test);
    
    int fd[2];
    pipe(fd);

    // 收集所有测试任务
    typedef struct {
        int test_type; // 0: CPU, 1: Memory, 2: Disk
        int index;
    } TestTask;

    int task_count = 0;
    if(cfg->cpu_test) task_count += cfg->cpu_test_times;
    if(cfg->mem_test) task_count += cfg->memory_test_times;
    if(cfg->disk_test) task_count += cfg->disk_test_times;
    if(cfg->device_test) task_count += cfg->device_test_times;

    log_info("Total test tasks: %d\n", task_count);
    progress_init(task_count);

    TestTask *tasks = malloc(sizeof(TestTask) * task_count);
    int task_index = 0;

    // 添加CPU测试任务
    if(cfg->cpu_test){
        for(int i = 0; i < cfg->cpu_test_times; i++){
            tasks[task_index].test_type = 0;
            tasks[task_index].index = i;
            task_index++;
        }
    }

    // 添加内存测试任务
    if(cfg->mem_test){
        for(int i = 0; i < cfg->memory_test_times; i++){
            tasks[task_index].test_type = 1;
            tasks[task_index].index = i;
            task_index++;
        }
    }

    // 添加磁盘测试任务
    if(cfg->disk_test){
        for(int i = 0; i < cfg->disk_test_times; i++){
            tasks[task_index].test_type = 2;
            tasks[task_index].index = i;
            task_index++;
        }
    }
    
    // 添加设备测试任务
    if(cfg->device_test){
        for(int i = 0; i < cfg->device_test_times; i++){
            tasks[task_index].test_type = 3;
            tasks[task_index].index = i;
            task_index++;
        }
    }

    // 子进程信息结构体
    typedef struct {
        pid_t pid;
        time_t start_time;
        int test_type;
        int task_index;
        int was_killed; // 标记是否被杀死
    } ProcessInfo;

    // 并发调度器
    int running = 0;
    int task_ptr = 0;
    ProcessInfo *processes = malloc(sizeof(ProcessInfo) * max_jobs);
    pid_t *completed_pids = malloc(sizeof(pid_t) * task_count);
    int *killed_flags = malloc(sizeof(int) * task_count); // 记录哪些进程被杀死
    int completed_count = 0;

    while(task_ptr < task_count || running > 0){
        // 启动新任务，直到达到max_jobs限制
        while(running < max_jobs && task_ptr < task_count){
            int current_task = task_ptr; // 保存当前任务索引
            pid_t pid = fork();
            if(pid == 0){
                close(fd[0]);
                // 子进程中禁用日志，避免使用父进程的log_fp
                Metrics metrics;
                
                switch(tasks[current_task].test_type){
                    case 0: // CPU test
                        metrics = cpu_test();
                        break;
                    case 1: // Memory test
                        metrics = mem_test();
                        break;
                    case 2: // Disk test
                        metrics = disk_test();
                        break;
                    case 3: //Device test
                        metrics = device_test();
                        break;
                    default:
                        metrics.result = 1;
                }
                
                write(fd[1], &metrics, sizeof(Metrics));
                close(fd[1]); // 关闭写端
                exit(metrics.result);
            } else if(pid > 0){
                // 记录子进程信息
                processes[running].pid = pid;
                processes[running].start_time = time(NULL);
                processes[running].test_type = tasks[current_task].test_type;
                processes[running].task_index = current_task;
                processes[running].was_killed = 0;
                log_debug("Started %s test process %d (task %d)\n", 
                         tasks[current_task].test_type == 0 ? "CPU" : 
                         tasks[current_task].test_type == 1 ? "Memory" : "Disk", 
                         pid, current_task);
                running++;
                task_ptr++;
            }
        }

        // 非阻塞轮询检查子进程状态
        for(int i = 0; i < running; i++){
            int status;
            pid_t child_pid = waitpid(processes[i].pid, &status, WNOHANG);
            
            if(child_pid > 0){
                // 子进程已完成
                log_debug("%s test process %d completed\n", 
                         processes[i].test_type == 0 ? "CPU" : 
                         processes[i].test_type == 1 ? "Memory" : "Disk", 
                         child_pid);
                completed_pids[completed_count] = child_pid;
                killed_flags[completed_count] = processes[i].was_killed;
                completed_count++;
                progress_update(completed_count);
                
                // 从运行列表中移除完成的进程
                for(int j = i; j < running - 1; j++){
                    processes[j] = processes[j + 1];
                }
                running--;
                i--; // 调整索引
            } else if(child_pid == 0){
                // 子进程仍在运行，检查是否超时
                if(timeout > 0){
                    time_t current_time = time(NULL);
                    if(current_time - processes[i].start_time >= timeout){
                        // 超时，杀死子进程
                        log_warn("%s test process %d timed out, killing...\n", 
                               processes[i].test_type == 0 ? "CPU" : 
                               processes[i].test_type == 1 ? "Memory" : "Disk", 
                               processes[i].pid);
                        processes[i].was_killed = 1;
                        kill(processes[i].pid, SIGKILL);
                        
                        // 等待子进程被杀死
                        waitpid(processes[i].pid, &status, 0);
                        
                        // 记录超时的进程
                        completed_pids[completed_count] = processes[i].pid;
                        killed_flags[completed_count] = 1;
                        completed_count++;
                        
                        // 从运行列表中移除
                        for(int j = i; j < running - 1; j++){
                            processes[j] = processes[j + 1];
                        }
                        running--;
                        i--; // 调整索引
                    }
                }
            }
        }

        // 短暂休眠，避免忙等
        if(running > 0){
            usleep(100000); // 100ms
        }
    }

    close(fd[1]);

    // 读取所有测试结果
    for(int i = 0; i < task_count; i++){
        if(killed_flags[i]){
            // 进程被杀死，直接生成失败结果
            Metrics error_metrics = {0};
            error_metrics.result = 1;
            error_metrics.execution_time = timeout;
            report_add_result_with_metrics(completed_pids[i], error_metrics);
        } else {
            // 进程正常完成，尝试读取结果
            Metrics metrics;
            ssize_t n = read(fd[0], &metrics, sizeof(Metrics));
            if(n == sizeof(Metrics)){
                // 正常读取到结果
                report_add_result_with_metrics(completed_pids[i], metrics);
            } else {
                // 读取失败，生成失败结果
                Metrics error_metrics = {0};
                error_metrics.result = 1;
                error_metrics.execution_time = timeout;
                report_add_result_with_metrics(completed_pids[i], error_metrics);
            }
        }
    }
    close(fd[0]);

    free(completed_pids);
    free(killed_flags);
    free(tasks);
    free(processes);
    
    // 记录测试完成信息
    progress_finish();
    log_info("All tests completed\n");

    report_summary();
    report_summary_json();
}