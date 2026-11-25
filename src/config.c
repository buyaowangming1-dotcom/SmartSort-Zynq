#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "config.h"

static char *trim(char *str)
{
    char *end;

    while(isspace((unsigned char)*str))
    {
        str++;
    }

    if(*str == '\0')
    {
        return str;
    }

    end = str+strlen(str)-1;

    while(end>str && isspace((unsigned char)*end))
    {
        end--;
    }

    *(end+1) ='\0';

    return str;
}

void config_default(AppConfig *config)
{
    if(config==NULL)
    {
        return;
    }
    config->refresh_interval=1;
    snprintf(config->log_file,sizeof(config->log_file),"%s","logs/smartsort.log");
}

int config_load(const char *filename, AppConfig *config)
{
    FILE *fp;
    char line[512];

    if(filename ==NULL || config ==NULL)
    {
        return -1;
    }

    fp =fopen(filename,"r");
    
    if(fp ==NULL)
    {
        return -1;
    }
    while (fgets(line,sizeof(line),fp)!=NULL)
    {
        char *key;
        char *value;
        char *equal;

        key =trim(line);
        if(*key =='\0'||*key =='#'||*key ==';')
        {
            continue;
        }
        equal = strchr(key, '=');

        if (equal == NULL)
        {
            continue;
        }
        *equal = '\0';

        key = trim(key);
        value = trim(equal + 1);

        if (strcmp(key, "refresh_interval") == 0)
        {
            char *endptr;
            long interval;
            errno = 0;
            interval = strtol(value, &endptr, 10);
            if (errno == 0 && endptr != value && *trim(endptr) == '\0' && interval >= 1 && interval <= 60)
            {
                config->refresh_interval = (int)interval;
            }
        }

        else if (strcmp(key, "log_file") == 0)
        {
            if (*value != '\0')
            {
                snprintf(config->log_file,sizeof(config->log_file),"%s",value);
            }
        }
    }
    fclose(fp);

    return 0;
}