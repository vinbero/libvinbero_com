#ifndef _VINBERO_COM_LOG_H
#define _VINBERO_COM_LOG_H

#include <string.h>

#define VINBERO_COM_LOG_LEVEL_TRACE 0
#define VINBERO_COM_LOG_LEVEL_DEBUG 1 
#define VINBERO_COM_LOG_LEVEL_INFO 2
#define VINBERO_COM_LOG_LEVEL_WARN 3
#define VINBERO_COM_LOG_LEVEL_ERROR 4
#define VINBERO_COM_LOG_LEVEL_FATAL 5

#define VINBERO_COM_LOG_FLAG_TRACE 1
#define VINBERO_COM_LOG_FLAG_DEBUG 2
#define VINBERO_COM_LOG_FLAG_INFO 4
#define VINBERO_COM_LOG_FLAG_WARN 8
#define VINBERO_COM_LOG_FLAG_ERROR 16
#define VINBERO_COM_LOG_FLAG_FATAL 32
#define VINBERO_COM_LOG_FLAG_ALL (VINBERO_COM_LOG_FLAG_TRACE | \
VINBERO_COM_LOG_FLAG_DEBUG | \
VINBERO_COM_LOG_FLAG_INFO |  \
VINBERO_COM_LOG_FLAG_WARN |  \
VINBERO_COM_LOG_FLAG_ERROR | \
VINBERO_COM_LOG_FLAG_FATAL)

#define VINBERO_COM_LOG_OPTION_COLOR 1
#define VINBERO_COM_LOG_OPTION_SOURCE 2

int vinbero_com_Log_init(int flag, int option);
void vinbero_com_Log_raw(int level, const char* source, int line, const char* format, ...);
void vinbero_com_Log_printLogLevelInfo(int flag);

#define __FILENAME__ strrchr("/" __FILE__, '/') + 1


#define VINBERO_COM_LOG_TRACE(...) vinbero_com_Log_raw(VINBERO_COM_LOG_LEVEL_TRACE, __FILENAME__, __LINE__, __VA_ARGS__)
#define VINBERO_COM_LOG_TRACE2(...) VINBERO_COM_LOG_TRACE("in %s()", __func__)
#define VINBERO_COM_LOG_DEBUG(...) vinbero_com_Log_raw(VINBERO_COM_LOG_LEVEL_DEBUG, __FILENAME__, __LINE__, __VA_ARGS__)
#define VINBERO_COM_LOG_INFO(...) vinbero_com_Log_raw(VINBERO_COM_LOG_LEVEL_INFO, __FILENAME__, __LINE__, __VA_ARGS__)
#define VINBERO_COM_LOG_WARN(...) vinbero_com_Log_raw(VINBERO_COM_LOG_LEVEL_WARN, __FILENAME__, __LINE__, __VA_ARGS__)
#define VINBERO_COM_LOG_ERROR(...) vinbero_com_Log_raw(VINBERO_COM_LOG_LEVEL_ERROR, __FILENAME__, __LINE__, __VA_ARGS__)
#define VINBERO_COM_LOG_FATAL(...) vinbero_com_Log_raw(VINBERO_COM_LOG_LEVEL_FATAL, __FILENAME__, __LINE__, __VA_ARGS__)

#endif
