#ifndef _VINBERO_COMMON_MODULE_H
#define _VINBERO_COMMON_MODULE_H

#include <dlfcn.h>
#include <fastdl.h>
#include <pthread.h>
#include <libgenc/genc_Generic.h>
#include <libgenc/genc_Tree.h>
#include <libgenc/genc_ArrayList.h>
#include "vinbero_common_Config.h"
#include "vinbero_common_Error.h"

struct vinbero_common_Module {
    const char* id;
    const char* name;
    const char* version;
    bool childrenRequired;
    struct fastdl_Handle dlHandle;
    union genc_Generic localModule;
    pthread_rwlock_t* rwLock;
    pthread_key_t* tlModuleKey;
    GENC_TREE_NODE(struct vinbero_common_Module, struct vinbero_common_Module*);
};

struct vinbero_common_Module_Ids {
    GENC_ARRAY_LIST(const char*);
};

int vinbero_common_Module_dlopen(struct vinbero_common_Module* module, struct vinbero_common_Config* config);

#define VINBERO_COMMON_MODULE_DLSYM(interface, dlHandle, functionName, ret) do { \
    if(fastdl_sym(dlHandle, #functionName, (void**)&(interface)->functionName) == -1) \
        *(ret) = VINBERO_COMMON_EUNKNOWN; \
    else \
        *(ret) = 0; \
} \
while(0)

#endif
