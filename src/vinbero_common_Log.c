#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include "vinbero_common_Log.h"

static int vinbero_common_Log_flag = 0; 
static int vinbero_common_Log_option = 0; 


void vinbero_common_Log_printLogLevelInfo(int flag) {
    if(flag & VINBERO_COMMON_LOG_FLAG_TRACE)
        VINBERO_COMMON_LOG_INFO("TRACE LEVEL LOGGING ENABLED");
    if(flag & VINBERO_COMMON_LOG_FLAG_DEBUG)
        VINBERO_COMMON_LOG_INFO("DEBUG LEVEL LOGGING ENABLED");
    if(flag & VINBERO_COMMON_LOG_FLAG_INFO)
        VINBERO_COMMON_LOG_INFO("INFO LEVEL LOGGING ENABLED");
    if(flag & VINBERO_COMMON_LOG_FLAG_WARN)
        VINBERO_COMMON_LOG_INFO("WARN LEVEL LOGGING ENABLED");
    if(flag & VINBERO_COMMON_LOG_FLAG_ERROR)
        VINBERO_COMMON_LOG_INFO("ERROR LEVEL LOGGING ENABLED");
    if(flag & VINBERO_COMMON_LOG_FLAG_FATAL)
        VINBERO_COMMON_LOG_INFO("FATAL LEVEL LOGGING ENABLED");
}

static const char* vinbero_common_Log_levelString(int level) {
    static const char* coloredLevelStrings[] = {
        "\x1B[37m[TRACE]\x1B[0m",
        "\x1B[36m[DEBUG]\x1B[0m",
        "\x1B[32m[INFO ]\x1B[0m",
        "\x1B[33m[WARN ]\x1B[0m",
        "\x1B[35m[ERROR]\x1B[0m",
        "\x1B[31m[FATAL]\x1B[0m",
    };

    static const char* levelStrings[] = {
        "[TRACE]",
        "[DEBUG]",
        "[INFO ]",
        "[WARN ]",
        "[ERROR]",
        "[FATAL]",
    };

    if(0 <= level && level < sizeof(levelStrings) / sizeof(const char*)) {
        if(vinbero_common_Log_option & VINBERO_COMMON_LOG_OPTION_COLOR)
            return coloredLevelStrings[level];
        else
            return levelStrings[level];
    }

    return "[?????]";
}

static int vinbero_common_Log_levelToFlag(int level) {
    static const int flagTable[] = {1, 2, 4, 8, 16, 32};
    if(0 <= level && level < sizeof(flagTable))
        return flagTable[level];
    else
        return 0;
}

int vinbero_common_Log_init(int flag, int option) {
    vinbero_common_Log_flag = flag;
    vinbero_common_Log_option = option;
    return 0;
}

void vinbero_common_Log_raw(int level, const char* source, int line, const char* format, ...) {
    if((vinbero_common_Log_levelToFlag(level) & vinbero_common_Log_flag) != 0) {
        time_t t = time(NULL);
        struct tm now;
        localtime_r(&t, &now);
        va_list args;
        va_start(args, format);
        flockfile(stderr);
        if(vinbero_common_Log_option & VINBERO_COMMON_LOG_OPTION_COLOR)
            fprintf(stderr, "\x1B[1;30m[%02d/%02d/%d/%02d:%02d:%02d]\x1B[0m ", now.tm_mday, now.tm_mon + 1, now.tm_year + 1900, now.tm_hour, now.tm_min, now.tm_sec);
        else
            fprintf(stderr, "[%02d/%02d/%d/%02d:%02d:%02d] ", now.tm_mday, now.tm_mon + 1, now.tm_year + 1900, now.tm_hour, now.tm_min, now.tm_sec);
        fprintf(stderr, "[%lu] ", syscall(SYS_gettid));
        fprintf(stderr, "%s ", vinbero_common_Log_levelString(level));
        if(vinbero_common_Log_option & VINBERO_COMMON_LOG_OPTION_SOURCE)
           fprintf(stderr, "%s: %d: ", source, line);
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");
        funlockfile(stderr);
        va_end(args);
    }
}
