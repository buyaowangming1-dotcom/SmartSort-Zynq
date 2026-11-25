#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "logger.h"

static FILE *log_file = NULL;

int logger_init(const char *filename)
{
    if (filename == NULL)
    {
        return -1;
    }

    log_file = fopen(filename, "a");

    if (log_file == NULL)
    {
        perror("fopen log file");
        return -1;
    }

    return 0;
}

static void logger_write(const char *level,
                         const char *format,
                         va_list args)
{
    time_t now;
    struct tm time_info;
    char time_buffer[32];

    now = time(NULL);

    if (localtime_r(&now, &time_info) == NULL)
    {
        return;
    }

    strftime(time_buffer,
             sizeof(time_buffer),
             "%Y-%m-%d %H:%M:%S",
             &time_info);

    printf("[%s] [%s] ", time_buffer, level);

    vprintf(format, args);

    printf("\n");

    if (log_file != NULL)
    {
        fprintf(log_file,
                "[%s] [%s] ",
                time_buffer,
                level);

        va_list file_args;
        va_copy(file_args, args);
        vfprintf(log_file, format, file_args);
        va_end(file_args);

        fprintf(log_file, "\n");
        fflush(log_file);
    }
}

void logger_info(const char *format, ...)
{
    va_list args;

    va_start(args, format);

    logger_write("INFO", format, args);

    va_end(args);
}

void logger_error(const char *format, ...)
{
    va_list args;

    va_start(args, format);

    logger_write("ERROR", format, args);

    va_end(args);
}

void logger_close(void)
{
    if (log_file != NULL)
    {
        fclose(log_file);
        log_file = NULL;
    }
}