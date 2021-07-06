#include "../include/logger.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct timespec Logger::time_start = { 0, 0 };

void Logger::printf (log_level_t log_level, const char* str, va_list arglist)
{
    char buf [1024];
    if (log_level > init_log_level)
        return;
    int s;
    size_t pos = 0;
    struct timespec time_cur;
    uint64_t delta_us;
    unsigned sec, ms, us;
    const char* esc_seq = "";
    clock_gettime (CLOCK_MONOTONIC_RAW, &time_cur);
    delta_us = (time_cur.tv_sec - time_start.tv_sec) * 1000000 +
            (time_cur.tv_nsec - time_start.tv_nsec) / 1000;
    sec = delta_us / 1000000;
    delta_us %= 1000000;
    ms = delta_us / 1000;
    us = delta_us % 1000;

    switch (log_level)
    {
        case llError:
            esc_seq = "\e[1;31m"; // RED
            break;
        case llWarning:
            esc_seq = "\e[1;33m"; // YELLOW
            break;
        case llInfo:
            esc_seq = "\e[32m"; // GREEN
            break;
        case llDebug:
            esc_seq = "\e[37m"; // LIGHT GRAY
            break;
    }

    s = snprintf (buf + pos, sizeof (buf) - pos, "%s%3u,%03u,%03u | %s | ", esc_seq, sec, ms, us, modName);
    if (s <= 0)
        return;
    pos += s;
    s = vsnprintf (buf + pos, sizeof (buf) - pos, str, arglist);
    if (s < 0)
        return;
    pos += s;
    s = snprintf (buf + pos, sizeof (buf) - pos, "\e[0m\n");
    if (s <= 0)
        return;

    // Печать в стандартный вывод
    ::printf ("%s", buf);
    // Напечатать всё, что зависло в буферах
    fflush (stdout);
}


void Logger::err  (const char* str, ...)
{
    va_list arglist;
    va_start (arglist, str);
    printf (llError, str, arglist);
    va_end (arglist);
}

void Logger::warn (const char* str, ...)
{
    va_list arglist;
    va_start (arglist, str);
    printf (llWarning, str, arglist);
    va_end (arglist);
}

void Logger::info (const char* str, ...)
{
    va_list arglist;
    va_start (arglist, str);
    printf (llInfo, str, arglist);
    va_end (arglist);
}

void Logger::dbg  (const char* str, ...)
{
    va_list arglist;
    va_start (arglist, str);
    printf (llDebug, str, arglist);
    va_end (arglist);
}

Logger::Logger ()
{
    if (!time_start.tv_sec && !time_start.tv_nsec)
        // Инициализация лога (времени отсчета)
        clock_gettime (CLOCK_MONOTONIC_RAW, &time_start);
}


