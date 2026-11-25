#ifndef SYSTEM_INFO_H
#define SYSTEM_INFO_H

typedef struct{
    int cpu_cores;
    double uptime;
    unsigned long memory_total;
    unsigned long memory_available;
} SystemInfo;

int system_info_get(SystemInfo *info);

#endif // SYSTEM_INFO_H