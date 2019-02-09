#ifndef _VINBERO_COM_DLSYM
#define _VINBERO_COM_DLSYM

#include <fastdl.h>
#include "vinbero_com_Error.h"
#include "vinbero_com_Status.h"

#define VINBERO_COM_DLSYM(dlHandle, functionName, out, ret) do { \
    FASTDL_SYM(dlHandle, functionName, out, ret); \
    if(*(ret) == -1) \
        *(ret) = VINBERO_COM_ERROR_DLSYM; \
    else \
        *(ret) = VINBERO_COM_STATUS_SUCCESS; \
} while(0)

#endif
