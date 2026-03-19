#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "logger.h"
#include <stdarg.h>

static FILE *log_fp = NULL; // 定义一个静态文件指针，用于写日志
static int debug_mode = 0; // 全局debug模式标志
static int quiet_mode = 0; // 全局quiet模式标志
static int progress_total = 0;
static int progress_width = 50;

void log_init(const char *filename)
{
    log_fp = fopen(filename, "a");  // 打开日志文件进行追加写入
    if (log_fp == NULL)
    {
        printf("log open failed\n");
    }
}

void log_info(const char *format, ...)
{
    if (!log_fp || quiet_mode) return; // 避免在子进程中使用，quiet模式下不输出INFO日志
    
    va_list args;
    time_t now = time(NULL);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "[%Y-%m-%d %H:%M:%S] INFO: ", localtime(&now));

    va_start(args, format);
    printf("%s", time_str);
    vprintf(format, args);

    if(log_fp) {
        fprintf(log_fp, "%s", time_str);
        vfprintf(log_fp, format, args);
    }

    va_end(args);
}

void log_error(const char *format, ...)
{
    if (!log_fp) return; // 避免在子进程中使用
    
    va_list args;
    time_t now = time(NULL);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "[%Y-%m-%d %H:%M:%S] ERROR: ", localtime(&now));

    va_start(args, format);
    printf("%s", time_str);
    vprintf(format, args);

    if(log_fp) {
        fprintf(log_fp, "%s", time_str);
        vfprintf(log_fp, format, args);
    }

    va_end(args);
}

void log_debug(const char *format, ...)
{
    if (!log_fp || quiet_mode) return; // 避免在子进程中使用，quiet模式下不输出DEBUG日志
    if (!debug_mode) return; // 只在debug模式下输出
    
    time_t now = time(NULL);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "[%Y-%m-%d %H:%M:%S] DEBUG: ", localtime(&now));

    // 输出到标准输出
    va_list args;
    va_start(args, format);
    printf("%s", time_str);
    vprintf(format, args);
    va_end(args);

    // 输出到日志文件
    if(log_fp) {
        va_list args_file;
        va_start(args_file, format);
        fprintf(log_fp, "%s", time_str);
        vfprintf(log_fp, format, args_file);
        va_end(args_file);
    }
}

void log_warn(const char *format, ...)
{
    if (!log_fp || quiet_mode) return; // 避免在子进程中使用，quiet模式下不输出WARN日志
    
    va_list args;
    time_t now = time(NULL);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "[%Y-%m-%d %H:%M:%S] WARN: ", localtime(&now));

    va_start(args, format);
    printf("%s", time_str);
    vprintf(format, args);

    if(log_fp) {
        fprintf(log_fp, "%s", time_str);
        vfprintf(log_fp, format, args);
    }

    va_end(args);
}

void log_close()
{
    if (log_fp)
        fclose(log_fp);  // 关闭日志文件
}

void log_set_debug(int enable)
{
    debug_mode = enable;
    if (enable) {
        log_info("Debug mode enabled\n");
    }
}

void log_set_quiet(int enable)
{
    quiet_mode = enable;
    if (enable) {
        log_info("Quiet mode enabled\n");
    }
}

// 进度条相关函数
void progress_init(int total)
{
    progress_total = total;
    printf("Progress: [");
    for (int i = 0; i < progress_width; i++) {
        printf(" ");
    }
    printf("] 0%%\r");
    fflush(stdout);
}

void progress_update(int current)
{
    if (progress_total == 0) return;
    
    int percent = (current * 100) / progress_total;
    int filled = (current * progress_width) / progress_total;
    
    printf("Progress: [");
    for (int i = 0; i < progress_width; i++) {
        if (i < filled) {
            printf("=");
        } else if (i == filled) {
            printf(">");
        } else {
            printf(" ");
        }
    }
    printf("] %d%%\r", percent);
    fflush(stdout);
}

void progress_finish()
{
    printf("Progress: [");
    for (int i = 0; i < progress_width; i++) {
        printf("=");
    }
    printf("] 100%%\n");
    fflush(stdout);
}