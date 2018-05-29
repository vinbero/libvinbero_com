#ifndef _VINBERO_COMMON_LOG_H
#define _VINBERO_COMMON_LOG_H

#define VINBERO_COMMON_LOG_LEVEL_TRACE 0
#define VINBERO_COMMON_LOG_LEVEL_DEBUG 1 
#define VINBERO_COMMON_LOG_LEVEL_INFO 2
#define VINBERO_COMMON_LOG_LEVEL_WARN 3
#define VINBERO_COMMON_LOG_LEVEL_ERROR 4
#define VINBERO_COMMON_LOG_LEVEL_FATAL 5

#define VINBERO_COMMON_LOG_FLAG_TRACE 1
#define VINBERO_COMMON_LOG_FLAG_DEBUG 2
#define VINBERO_COMMON_LOG_FLAG_INFO 4
#define VINBERO_COMMON_LOG_FLAG_WARN 8
#define VINBERO_COMMON_LOG_FLAG_ERROR 16
#define VINBERO_COMMON_LOG_FLAG_FATAL 32
#define VINBERO_COMMON_LOG_FLAG_ALL (VINBERO_COMMON_LOG_FLAG_TRACE | \
VINBERO_COMMON_LOG_FLAG_DEBUG | \
VINBERO_COMMON_LOG_FLAG_INFO |  \
VINBERO_COMMON_LOG_FLAG_WARN |  \
VINBERO_COMMON_LOG_FLAG_ERROR | \
VINBERO_COMMON_LOG_FLAG_FATAL)

extern int vinbero_common_Log_flag;

void vinbero_common_Log_raw(int flag, int level, const char* source, int line, const char* format, ...);

#define VINBERO_COMMON_LOG_TRACE(...) vinbero_common_Log_raw(vinbero_common_Log_flag, VINBERO_COMMON_LOG_LEVEL_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define VINBERO_COMMON_LOG_TRACE2(...) VINBERO_COMMON_LOG_TRACE("in %s()", __FUNCTION__)
#define VINBERO_COMMON_LOG_DEBUG(...) vinbero_common_Log_raw(vinbero_common_Log_flag, VINBERO_COMMON_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define VINBERO_COMMON_LOG_INFO(...) vinbero_common_Log_raw(vinbero_common_Log_flag, VINBERO_COMMON_LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define VINBERO_COMMON_LOG_WARN(...) vinbero_common_Log_raw(vinbero_common_Log_flag, VINBERO_COMMON_LOG_LEVEL_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define VINBERO_COMMON_LOG_ERROR(...) vinbero_common_Log_raw(vinbero_common_Log_flag, VINBERO_COMMON_LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define VINBERO_COMMON_LOG_FATAL(...) vinbero_common_Log_raw(vinbero_common_Log_flag, VINBERO_COMMON_LOG_LEVEL_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#endif
