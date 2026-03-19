#ifndef LOGGER_H
#define LOGGER_H

void log_init(const char *filename);
void log_info(const char *format,...);
void log_error(const char *format,...);
void log_debug(const char *format,...);
void log_warn(const char *format,...);
void log_close();

// 进度条相关函数
void progress_init(int total);
void progress_update(int current);
void progress_finish();

// Debug模式控制
void log_set_debug(int enable);
// Quiet模式控制
void log_set_quiet(int enable);

#endif