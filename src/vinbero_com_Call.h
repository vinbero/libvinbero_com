#ifndef _VINBERO_COM_CALL
#define _VINBERO_COM_CALL

#include <vinbero_com/vinbero_com_Status.h>

#define VINBERO_COM_CALL(iName, fName, module, ret, ...) do { \
    struct vinbero_iface_ ## iName iface; \
    VINBERO_COM_DLSYM(&(module)->dlHandle, "vinbero_iface_"#iName"_"#fName, &iface.vinbero_iface_ ## iName ## _ ## fName, ret); \
    if(*(ret) < 0) { \
        VINBERO_COM_LOG_WARN("VINBERO_%s_DLSYM() failed at module %s, with function %s", #iName, (module)->id, "vinbero_iface_"#iName"_"#fName); \
        break; \
    } \
    if(((*ret) = iface.vinbero_iface_ ## iName ## _ ## fName (__VA_ARGS__)) < VINBERO_COM_STATUS_SUCCESS) \
        VINBERO_COM_LOG_WARN("vinbero_iface_%s_%s() failed at module %s", #iName, #fName, (module)->id); \
} while(0)

#define VINBERO_COM_CALL2(iface, iName, fName, module, ret, ...) do { \
    if((iface)->vinbero_iface_ ## iName ## _ ## fName == NULL) { \
        VINBERO_COM_LOG_WARN("vinbero_iface_%s_%s() is NULL at module %s", #iName, #fName, (module)->id); \
        break; \
    } \
    if(((*ret) = (iface)->vinbero_iface_ ## iName ## _ ## fName (__VA_ARGS__)) < VINBER_COM_STATUS_SUCCESS) \
        VINBERO_COM_LOG_WARN("vinbero_iface_%s_%s() failed at module %s", #iName, #fName, (module)->id); \
} while(0)

#endif
