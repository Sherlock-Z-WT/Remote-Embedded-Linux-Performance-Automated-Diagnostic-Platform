#include "logger.h"
#include "device.h"
#include "test_runner.h"
#include "config.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

// 全局变量，控制是否开启debug模式
int debug_mode = 0;
int quiet_mode = 0;

int main(int argc,char *argv[]){
    int timeout = 0;
    int opt;
    int test_count = 6;
    int max_jobs = 5;
    Config cfg;
    cfg.test_count = test_count;
    cfg.cpu_test = 1;
    cfg.mem_test = 1;
    cfg.disk_test = 1;
    cfg.cpu_test_times = 3;
    cfg.memory_test_times = 2;
    cfg.disk_test_times = 1;
    cfg.device_test = 1;
    cfg.device_test_times = 1;

    // 处理长选项
    struct option long_options[] = {
        {"debug", no_argument, &debug_mode, 1},
        {"quiet", no_argument, &quiet_mode, 1},
        {"config", required_argument, 0, 'c'},
        {0, 0, 0, 0}
    };
    
    int option_index = 0;
    while((opt = getopt_long(argc, argv, "n:j:t:c:q", long_options, &option_index)) != -1){
        switch(opt){
            case 't':
                timeout = atoi(optarg);
                break;
            case 'j':
                max_jobs = atoi(optarg);
                break;
            case 'n':
                cfg.test_count = atoi(optarg);
                break;
            case 'c':
                // 加载配置文件
                if(load_config(optarg, &cfg) != 0){
                    printf("Failed to load config file: %s\n", optarg);
                    return 1;
                }
                break;
            case 'q':
                quiet_mode = 1;
                break;
            case 0:
                // 长选项处理
                break;
            default:
                printf("Usage: %s [-n test_count] [-j max_jobs] [-t timeout] [-c config_file] [-q] [--debug] [--quiet]\n",argv[0]);
                return 1;
        }
    }
    log_init("log.txt");
    log_set_debug(debug_mode);
    log_set_quiet(quiet_mode);

    run_tests(&cfg,max_jobs,timeout);
    log_close();
    return 0;
}