#include <stdio.h>
#include <string.h>
#include "config.h"

int load_config(const char *filename,Config *cfg){
    FILE *fp = fopen(filename,"r");
    if(fp == NULL){
        printf("config open failed\n");
        return -1;
    }
    char line[128];
    while(fgets(line,sizeof(line),fp)){
        if(strncmp(line,"cpu_test_times=",15) == 0){
            sscanf(line+15,"%d",&cfg->cpu_test_times);
        }

        if(strncmp(line,"memory_test_times=",18) == 0){
            sscanf(line+18,"%d",&cfg->memory_test_times);
        }
    }
    fclose(fp);
    return 0;
}