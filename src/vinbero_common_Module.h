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
#include "vinbero_common_Error.h"
#include "vinbero_common_Status.h"

struct vinbero_common_Module {
    const char* name;
    const char* version;
    bool childrenRequired;
    struct vinbero_common_Config* config;
    const char* id;
    struct fastdl_Handle dlHandle;
    union genc_Generic localModule;
    void* arg; // arg from parentModule
    GENC_TREE_NODE(struct vinbero_common_Module, struct vinbero_common_Module*);
};

void vinbero_common_Module_init(struct vinbero_common_Module* module, const char* name, const char* version, bool childrenRequired);

#define VINBERO_COMMON_MODULE_DLOPEN(module, ret) do { \
    *(ret) = VINBERO_COMMON_STATUS_SUCCESS; \
    struct vinbero_common_Object* paths; \
    VINBERO_COMMON_CONFIG_MGET_REQ((module)->config, module, "paths", ARRAY, &paths); \
    if(paths == NULL) { \
        *(ret) = VINBERO_COMMON_ERROR_INVALID_CONFIG; \
        break; \
    } \
    if(GENC_TREE_NODE_CHILD_COUNT(paths) < 1) { \
        *(ret) = VINBERO_COMMON_ERROR_INVALID_CONFIG; \
        break; \
    } \
    GENC_TREE_NODE_FOR_EACH_CHILD(paths, index) { \
        struct vinbero_common_Object* path = GENC_TREE_NODE_GET_CHILD(paths, index); \
        if(!VINBERO_COMMON_OBJECT_IS_CONSTRING(path)) { \
            *(ret) = VINBERO_COMMON_ERROR_INVALID_CONFIG; \
	    break; \
        } \
        if(fastdl_open(&(module)->dlHandle, VINBERO_COMMON_OBJECT_CONSTRING(path), RTLD_LAZY | RTLD_GLOBAL) == -1) { \
            *(ret) = VINBERO_COMMON_ERROR_DLOPEN; \
            break; \
        } \
    } \
} while(0)

#define VINBERO_COMMON_MODULE_DLSYM(interface, dlHandle, functionName, ret) do { \
    (interface)->functionName = NULL; \
    VINBERO_COMMON_DLSYM(dlHandle, #functionName, (void**)&(interface)->functionName, ret); \
} while(0)

#endif
