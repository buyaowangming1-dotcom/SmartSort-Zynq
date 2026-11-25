#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "system_info.h"
#include "logger.h"
#include "config.h"

static volatile sig_atomic_t running = 1;

/* 信号处理函数 */
static void signal_handler(int signum)
{
    (void)signum;
    running = 0;
}

/* 显示系统信息 */
static void display_system_info(const SystemInfo *info)
{
    if (info == NULL)
    {
        return;
    }

    printf("====================================\n");
    printf("       SmartSort Linux Monitor      \n");
    printf("====================================\n");

    printf("CPU Cores       : %d\n", info->cpu_cores);

    printf("System Uptime   : %.2f seconds\n",
           info->uptime);

    printf("Memory Total    : %lu MB\n",
           info->memory_total / 1024);

    printf("Memory Available: %lu MB\n",
           info->memory_available / 1024);

    if (info->memory_available <= info->memory_total)
    {
        printf("Memory Used     : %lu MB\n",
               (info->memory_total -
                info->memory_available) / 1024);
    }

    printf("====================================\n");
}

/* 主程序 */
int main(void)
{
    AppConfig config;
    SystemInfo info;

    /* 注册信号处理函数 */
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    /* 加载默认配置 */
    config_default(&config);

    /* 尝试加载配置文件 */
    if (config_load("config/smartsort.conf", &config) != 0)
    {
        printf("Config file unavailable, use default config.\n");
    }

    /* 初始化日志 */
    if (logger_init(config.log_file) != 0)
    {
        printf("Warning: log initialization failed.\n");
    }

    logger_info("SmartSortMonitor started.");

    logger_info("Refresh interval: %d s",
                config.refresh_interval);

    while (running)
    {
        if (system_info_get(&info) == 0)
        {
            printf("\033[2J\033[H");

            display_system_info(&info);

            logger_info("System information updated.");
        }
        else
        {
            logger_error("Failed to obtain system information.");
        }

        printf("Refresh interval: %d s\n",
               config.refresh_interval);

        printf("Press Ctrl+C to exit.\n");

        sleep(config.refresh_interval);
    }

    logger_info("SmartSortMonitor stopped.");

    logger_close();

    printf("\nSmartSortMonitor exited normally.\n");

    return 0;
}