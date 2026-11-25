#ifndef CONFIG_H
#define CONFIG_H

typedef struct
{
    int refresh_interval;
    char log_file[256];
} AppConfig;

void config_default(AppConfig *config);

int config_load(const char *filename, AppConfig *config);

#endif