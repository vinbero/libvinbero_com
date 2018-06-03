#ifndef _VINBERO_COMMON_CALL
#define _VINBERO_COMMON_CALL
#define VINBERO_COMMON_CALL(iName, fName, module, ret, ...) do { \
    struct vinbero_Interface_ ## iName interface; \
    VINBERO_COMMON_DLSYM(&(module)->dlHandle, "vinbero_Interface_"#iName"_"#fName, &interface.vinbero_Interface_ ## iName ## _ ## fName, ret); \
    if(*ret < 0) { \
        VINBERO_COMMON_LOG_WARN("VINBERO_%s_DLSYM() failed at module %s, with function %s", #iName, (module)->id, "vinbero_Interface_"#iName"_"#fName); \
        break; \
    } \
    if(((*ret) = interface.vinbero_Interface_ ## iName ## _ ## fName (__VA_ARGS__)) < 0) \
        VINBERO_COMMON_LOG_WARN("vinbero_Interface_%s_%s() failed at module %s", #iName, #fName, (module)->id); \
} while(0)

#define VINBERO_COMMON_CALL2(interface, iName, fName, module, ret, ...) do { \
    if((interface)->vinbero_Interface_ ## iName ## _ ## fName == NULL) { \
        VINBERO_COMMON_LOG_WARN("vinbero_Interface_%s_%s() is NULL at module %s", #iName, #fName, (module)->id); \
        break; \
    } \
    if(((*ret) = (interface)->vinbero_Interface_ ## iName ## _ ## fName (__VA_ARGS__)) < 0) \
        VINBERO_COMMON_LOG_WARN("vinbero_Interface_%s_%s() failed at module %s", #iName, #fName, (module)->id); \
} while(0)

#endif
