#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* 获取 CPU 核心数量 */
int get_cpu_cores(void)
{
    FILE *fp;
    char line[256];
    int cores = 0;

    fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL)
    {
        perror("open /proc/cpuinfo");
        return -1;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (strncmp(line, "processor", 9) == 0)
        {
            cores++;
        }
    }

    fclose(fp);
    return cores;
}

/* 获取系统运行时间 */
double get_uptime(void)
{
    FILE *fp;
    double uptime = 0;

    fp = fopen("/proc/uptime", "r");
    if (fp == NULL)
    {
        perror("open /proc/uptime");
        return -1;
    }

    fscanf(fp, "%lf", &uptime);
    fclose(fp);

    return uptime;
}

/* 获取内存信息 */
void get_memory_info(unsigned long *total, unsigned long *available)
{
    FILE *fp;
    char line[256];

    *total = 0;
    *available = 0;

    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL)
    {
        perror("open /proc/meminfo");
        return;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        if (sscanf(line, "MemTotal: %lu kB", total) == 1)
            continue;

        if (sscanf(line, "MemAvailable: %lu kB", available) == 1)
            continue;
    }

    fclose(fp);
}

/* 主函数 */
int main(void)
{
    int cores;
    double uptime;
    unsigned long total, available;

    printf("====================================\n");
    printf("       SmartSort Linux Monitor      \n");
    printf("====================================\n");

    while (1)
    {
        cores = get_cpu_cores();
        uptime = get_uptime();

        get_memory_info(&total, &available);

        printf("\033[2J\033[H");  // 清屏并将光标移到左上角

        printf("====================================\n");
        printf("       SmartSort Linux Monitor      \n");
        printf("====================================\n");

        printf("CPU Cores       : %d\n", cores);
        printf("System Uptime   : %.2f seconds\n", uptime);

        if (total > 0)
        {
            printf("Memory Total    : %lu MB\n", total / 1024);
            printf("Memory Available: %lu MB\n", available / 1024);
            printf("Memory Used     : %lu MB\n",
                   (total - available) / 1024);
        }
        else
        {
            printf("Memory Info     : unavailable\n");
        }

        printf("====================================\n");
        printf("Refresh interval: 1 second\n");
        printf("Press Ctrl+C to exit\n");

        sleep(1);
    }

    return 0;
}