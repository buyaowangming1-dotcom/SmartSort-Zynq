#include <stdio.h>
#include <string.h>

#include "system_info.h"

static int get_cpu_cores(void) {
    FILE *fp;
    char line[256];
    int cores = 0;

    fp=fopen("/proc/cpuinfo", "r");
    if(fp==NULL){
        perror("open /proc/cpuinfo");
        return -1;
    }

    while(fgets(line,sizeof(line),fp)!=NULL)
    {
        if (strncmp(line, "processor", 9) == 0)
        {
            cores++;
        }
    }
    fclose(fp);
    return cores;
}

static int get_uptime(double *uptime)
{
    FILE *fp;

    if(uptime ==NULL)
    {
        return -1;
    }
    fp =fopen("/proc/uptime", "r");

    if(fp == NULL)
    {
        perror("fopen /proc/uptime");
        return -1;
    }

    if(fscanf(fp, "%lf", uptime)!=1)
    {
        fclose(fp);
        return -1;
    }

    fclose(fp);

    return 0;

}

static int get_memory_info(unsigned long *total, unsigned long *available) {
    FILE *fp;
    char line[256];

    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        perror("open /proc/meminfo");
        return -1;
    }

    *total=0;
    *available=0;

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line, "MemTotal: %lu kB", total) == 1) 
        {
            continue;
        }
        if (sscanf(line,"MemAvailable: %lu kB", available) ==1)
        {
            continue;
        }
  
    }
    fclose(fp);
    if(*total==0)
    {
        return -1;
    }
    return 0;
}

int system_info_get(SystemInfo *info)
{
    if (info == NULL)
    {
        return -1;
    }

    memset(info, 0, sizeof(SystemInfo));

    info->cpu_cores = get_cpu_cores();

    if (info->cpu_cores < 0)
    {
        return -1;
    }

    if (get_uptime(&info->uptime) != 0)
    {
        return -1;
    }

    if (get_memory_info(&info->memory_total,
                        &info->memory_available) != 0)
    {
        return -1;
    }

    return 0;
}