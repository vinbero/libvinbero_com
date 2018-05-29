#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "vinbero_common_Log.h"

static int vinbero_common_Log_flag = 0; 

static pthread_mutex_t vinbero_common_Log_mutex;
static bool vinbero_common_Log_mutexInitialized = false;


static const char* vinbero_common_Log_levelString(int level) {
    static const char* levelStrings[] = {
        "\x1B[37mTRACE\x1B[0m",
        "\x1B[36mDEBUG\x1B[0m",
        "\x1B[32mINFO\x1B[0m",
        "\x1B[33mWARN\x1B[0m",
        "\x1B[35mERROR\x1B[0m",
        "\x1B[31mFATAL\x1B[0m",
    };
    if(0 <= level && level < sizeof(levelStrings))
        return levelStrings[level];
    return "UNKNOWN";
}

static int vinbero_common_Log_levelToFlag(int level) {
    static const int flagTable[] = {1, 2, 4, 8, 16, 32};
    if(0 <= level && level < sizeof(flagTable))
        return flagTable[level];
    else
        return 0;
}

int vinbero_common_Log_init(int flag) {
    vinbero_common_Log_flag = flag;
    if(pthread_mutex_init(&vinbero_common_Log_mutex, NULL) == -1)
        return -errno;
    vinbero_common_Log_mutexInitialized = true;
    return 0;
}

void vinbero_common_Log_raw(int level, const char* source, int line, const char* format, ...) {
    if(!vinbero_common_Log_mutexInitialized)
        return;
    pthread_mutex_lock(&vinbero_common_Log_mutex);
    if((vinbero_common_Log_levelToFlag(level) & vinbero_common_Log_flag) != 0) {
        time_t t = time(NULL);
        struct tm now;
        localtime_r(&t, &now);
        fprintf(stderr, "\x1B[1;30m[%02d/%02d/%d/%02d:%02d:%02d]\x1B[0m ", now.tm_mday, now.tm_mon + 1, now.tm_year + 1900, now.tm_hour, now.tm_min, now.tm_sec);
        fprintf(stderr, "%s %s: %d: ", vinbero_common_Log_levelString(level), source, line);
        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
        fprintf(stderr, "\n");
    }
    pthread_mutex_unlock(&vinbero_common_Log_mutex);
}
