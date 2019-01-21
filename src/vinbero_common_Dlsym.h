#ifndef _VINBERO_COMMON_DLSYM
#define _VINBERO_COMMON_DLSYM

#include <fastdl.h>
#include "vinbero_common_Error.h"

#define VINBERO_COMMON_DLSYM(dlHandle, functionName, out, ret) do { \
    FASTDL_SYM(dlHandle, functionName, out, ret); \
    if(*(ret) == -1) \
        *(ret) = VINBERO_COMMON_ERROR_DLSYM; \
    else \
        *(ret) = 0; \
} while(0)

#endif
