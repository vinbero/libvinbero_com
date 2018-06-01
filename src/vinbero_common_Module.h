#ifndef _VINBERO_COMMON_MODULE_H
#define _VINBERO_COMMON_MODULE_H

#include <dlfcn.h>
#include <fastdl.h>
#include <pthread.h>
#include <libgenc/genc_Generic.h>
#include <libgenc/genc_Tree.h>
#include <libgenc/genc_ArrayList.h>
#include "vinbero_common_Config.h"
#include "vinbero_common_Dlsym.h"

struct vinbero_common_Module {
    const char* id;
    const char* name;
    const char* version;
    bool childrenRequired;
    struct vinbero_common_Config* config;
    struct fastdl_Handle dlHandle;
    union genc_Generic localModule;
    pthread_rwlock_t* rwLock;
    pthread_key_t* tlModuleKey;
    GENC_TREE_NODE(struct vinbero_common_Module, struct vinbero_common_Module*);
};

struct vinbero_common_Module_Ids {
    GENC_ARRAY_LIST(const char*);
};

int vinbero_common_Module_dlopen(struct vinbero_common_Module* module);

#define VINBERO_COMMON_MODULE_DLSYM(interface, dlHandle, functionName, ret) \
VINBERO_COMMON_DLSYM(dlHandle, #functionName, (void**)&(interface)->functionName, ret)

#endif
