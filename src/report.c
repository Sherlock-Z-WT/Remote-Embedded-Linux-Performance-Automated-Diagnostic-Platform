#include <stdio.h>
#include <time.h>
#include "report.h"

static FILE *fp;
static FILE *json_fp;
static int total = 0;
static int pass = 0;
static int fail = 0;
static time_t start_time;
static time_t end_time;
static int first_result = 1;

void report_init(){
    // 创建report目录
    system("mkdir -p report");
    fp = fopen("report/test_report.txt","w");
    if(!fp){
        perror("report file open failed");
        return;
    }
    total = 0;
    pass = 0;
    fail = 0;
    start_time = time(NULL);
    fprintf(fp,"TEST REPORT\n");
    fprintf(fp,"==========\n\n");
}

void report_init_json(){
    json_fp = fopen("report/test_report.json","w");
    if(!json_fp){
        perror("JSON report file open failed");
        return;
    }
    first_result = 1;
    fprintf(json_fp,"{\n");
    fprintf(json_fp,"  \"timestamp\": \"%s\",\n", ctime(&start_time));
    fprintf(json_fp,"  \"tests\": [\n");
}

void report_add_result(int pid,int result){
    total++;
    if(result == 0){
        pass++;
        fprintf(fp,"Process %d PASS\n",pid);
    }else{
        fail++;
        fprintf(fp,"Prcess %d FAIL\n",pid);
    }
    
    // JSON报告
    if(json_fp){
        if(!first_result){
            fprintf(json_fp,",\n");
        }
        first_result = 0;
        fprintf(json_fp,"    {\n");
        fprintf(json_fp,"      \"pid\": %d,\n", pid);
        fprintf(json_fp,"      \"result\": %s,\n", result == 0 ? "\"PASS\"" : "\"FAIL\"" );
        fprintf(json_fp,"      \"metrics\": {\n");
        fprintf(json_fp,"        \"execution_time\": 0,\n");
        fprintf(json_fp,"        \"cpu_usage\": 0,\n");
        fprintf(json_fp,"        \"memory_usage\": 0,\n");
        fprintf(json_fp,"        \"disk_usage\": 0,\n");
        fprintf(json_fp,"        \"io_ops\": 0\n");
        fprintf(json_fp,"      }\n");
        fprintf(json_fp,"    }");
    }
}

void report_add_result_with_metrics(int pid,Metrics metrics){
    total++;
    if(metrics.result == 0){
        pass++;
        fprintf(fp,"Process %d PASS\n",pid);
    }else{
        fail++;
        fprintf(fp,"Prcess %d FAIL\n",pid);
    }
    
    // 输出详细的metrics信息
    fprintf(fp,"  Execution time: %.4f seconds\n",metrics.execution_time);
    fprintf(fp,"  CPU usage: %d%%\n",metrics.cpu_usage);
    fprintf(fp,"  Memory usage: %d MB\n",metrics.memory_usage);
    fprintf(fp,"  Disk usage: %d MB\n",metrics.disk_usage);
    fprintf(fp,"  I/O operations: %d\n\n",metrics.io_ops);
    
    // JSON报告
    if(json_fp){
        if(!first_result){
            fprintf(json_fp,",\n");
        }
        first_result = 0;
        fprintf(json_fp,"    {\n");
        fprintf(json_fp,"      \"pid\": %d,\n", pid);
        fprintf(json_fp,"      \"result\": %s,\n", metrics.result == 0 ? "\"PASS\"" : "\"FAIL\"" );
        fprintf(json_fp,"      \"metrics\": {\n");
        fprintf(json_fp,"        \"execution_time\": %.4f,\n", metrics.execution_time);
        fprintf(json_fp,"        \"cpu_usage\": %d,\n", metrics.cpu_usage);
        fprintf(json_fp,"        \"memory_usage\": %d,\n", metrics.memory_usage);
        fprintf(json_fp,"        \"disk_usage\": %d,\n", metrics.disk_usage);
        fprintf(json_fp,"        \"io_ops\": %d\n", metrics.io_ops);
        fprintf(json_fp,"      }\n");
        fprintf(json_fp,"    }");
    }
}

void report_summary(){
    end_time = time(NULL);
    if(fp){
        fprintf(fp,"\n");
        fprintf(fp,"TOTAL:%d\n",total);
        fprintf(fp,"PASS:%d\n",pass);
        fprintf(fp,"FAIL:%d\n",fail);

        fclose(fp);
    }
}

void report_summary_json(){
    if(json_fp){
        end_time = time(NULL);
        fprintf(json_fp,"\n");
        fprintf(json_fp,"  ],\n");
        fprintf(json_fp,"  \"summary\": {\n");
        fprintf(json_fp,"    \"total\": %d,\n", total);
        fprintf(json_fp,"    \"pass\": %d,\n", pass);
        fprintf(json_fp,"    \"fail\": %d,\n", fail);
        fprintf(json_fp,"    \"duration\": %.2f,\n", difftime(end_time, start_time));
        fprintf(json_fp,"    \"status\": %s\n", fail == 0 ? "\"SUCCESS\"" : "\"FAILURE\"" );
        fprintf(json_fp,"  }\n");
        fprintf(json_fp,"}\n");
        fclose(json_fp);
    }
}

void report_flush(){
    if(fp){
        fflush(fp);
    }
}