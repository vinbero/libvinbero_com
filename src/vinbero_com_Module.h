#ifndef _VINBERO_COM_MODULE_H
#define _VINBERO_COM_MODULE_H

#include <dlfcn.h>
#include <fastdl.h>
#include <pthread.h>
#include <libgenc/genc_Generic.h>
#include <libgenc/genc_Tree.h>
#include <libgenc/genc_ArrayList.h>
#include "vinbero_com_Config.h"
#include "vinbero_com_Dlsym.h"
#include "vinbero_com_Error.h"
#include "vinbero_com_Status.h"

struct vinbero_com_Module {
    const char* name;
    const char* version;
    bool childrenRequired;
    struct vinbero_com_Config* config;
    const char* id;
    const char* path;
    struct fastdl_Handle dlHandle;
    union genc_Generic localModule;
    void* arg; // arg from parentModule
    GENC_TREE_NODE(struct vinbero_com_Module, struct vinbero_com_Module*);
};

void vinbero_com_Module_init(struct vinbero_com_Module* module, const char* name, const char* version, bool childrenRequired);

#define VINBERO_COM_MODULE_DLSYM(interface, dlHandle, functionName, ret) do { \
    (interface)->functionName = NULL; \
    VINBERO_COM_DLSYM(dlHandle, #functionName, (void**)&(interface)->functionName, ret); \
} while(0)

#endif
