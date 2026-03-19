#include <stdio.h>
#include "device.h"
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

static DeviceInfo device_info = {0};

int device_init(const char *ip,int port,const char *username,const char *password){
    strncpy(device_info.ip,ip,sizeof(device_info.ip));
    device_info.port = port;
    strncpy(device_info.username,username,sizeof(device_info.username));
    strncpy(device_info.password,password,sizeof(device_info.password));
    device_info.status = DEVICE_DISCONNECTED;
    return 0;
}

int device_connect(){
    printf("Connecting to %s:%d...\n",device_info.ip,device_info.port);
    sleep(1);
    printf("Device connected\n");
    device_info.status = DEVICE_CONNECTED;
    return 0;
}

int device_disconnect(){
    if(device_info.status == DEVICE_CONNECTED){
        printf("Disconnecting from device...\n");
        sleep(1);
        printf("Device disconnected\n");
        device_info.status = DEVICE_DISCONNECTED;

    }
    return 0;
}

int device_send_command(const char *cmd,char *response,int response_size){
    if(device_info.status != DEVICE_CONNECTED){
        return -1;
    }
    printf("Sending command:%s\n",cmd);
    snprintf(response,response_size,"Command executed successfully");
    return 0;
}

int device_get_metrics(DeviceMetrics *metrics){
    if(device_info.status != DEVICE_CONNECTED){
        return -1;
    }
    metrics->cpu_usage = 36.3;
    metrics->memory_used = 256;
    metrics->memory_total = 512;
    metrics->disk_used = 128;
    metrics->disk_total = 256;
    metrics->uptime = 2990;
    metrics->process_count = 26;
    return 0;
}

DeviceStatus device_get_status(){
    return device_info.status;
}