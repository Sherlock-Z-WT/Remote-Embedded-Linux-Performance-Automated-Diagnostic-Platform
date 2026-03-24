#ifndef REPORT_H
#define REPORT_H
#include "metrics.h"

void report_init();
void report_init_json();
void report_add_result(int pid,int result);
void report_add_result_with_metrics(int pid,Metrics metrics);
void report_summary();
void report_summary_json();
void report_flush();

#endif