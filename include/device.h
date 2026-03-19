#ifndef DEVICE_H
#define DEVICE_H

typedef enum{
    DEVICE_DISCONNECTED,
    DEVICE_CONNECTED,
    DEVICE_ERROR
}DeviceStatus;

typedef struct{
    char ip[32];
    int port;
    char username[32];
    char password[32];
    DeviceStatus status;
}DeviceInfo;

typedef struct{
    float cpu_usage;
    int memory_used;
    int memory_total;
    int disk_used;
    int disk_total;
    int uptime;
    int process_count;
}DeviceMetrics;

int device_init(const char *ip,int port,const char *username,const char *password);
int device_connect();
int device_disconnect();
int device_send_command(const char *cmd,char *response,int response_size);
int device_get_metrics(DeviceMetrics *metrics);
DeviceStatus device_get_status();

#endif