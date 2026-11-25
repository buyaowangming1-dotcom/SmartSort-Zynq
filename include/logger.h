#ifndef LOGGER_H
#define LOGGER_H

int logger_init(const char *filename);

void logger_info(const char *format, ...);

void logger_error(const char *format, ...);

void logger_close(void);

#endif