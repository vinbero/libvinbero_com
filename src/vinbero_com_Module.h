#ifndef _VINBERO_COM_MODULE_H
#define _VINBERO_COM_MODULE_H

#include <dlfcn.h>
#include <fastdl.h>
#include <pthread.h>
#include <libgenc/genc_Generic.h>
#include <libgenc/genc_Tree.h>
#include "vinbero_com_Config.h"
#include "vinbero_com_Dlsym.h"
#include "vinbero_com_Error.h"
#include "vinbero_com_Status.h"

struct vinbero_com_Module {
    struct vinbero_com_Config* config;
    const char* id;
    const char* path;
    struct fastdl_Handle dlHandle;
    union genc_Generic localModule;
    void* arg; // arg from parentModule
    GENC_TREE_NODE(struct vinbero_com_Module, struct vinbero_com_Module*);
};

#define VINBERO_COM_MODULE_DLSYM(iface, dlHandle, funcName, ret) do { \
    (iface)->funcName = NULL; \
    VINBERO_COM_DLSYM(dlHandle, #funcName, (void**)&(iface)->funcName, ret); \
} while(0)

#define VINBERO_COM_MODULE_META(name, type, value) \
type vinbero_com_Module_Meta_##name() { \
    return value; \
}

#define VINBERO_COM_MODULE_META_GET(module, name, value, ret) do { \
    typeof(*(value)) (*meta)(); \
    VINBERO_COM_DLSYM(&(module)->dlHandle, "vinbero_com_Module_Meta_"name, (void**)&meta, ret); \
    if(*(ret) < VINBERO_COM_STATUS_SUCCESS) \
        break; \
    if(meta != NULL) \
        *(value) = meta(); \
} while(0)

#define VINBERO_COM_MODULE_META_NAME(name) \
VINBERO_COM_MODULE_META(NAME, const char*, name)

#define VINBERO_COM_MODULE_META_LICENSE(license) \
VINBERO_COM_MODULE_META(LICENSE, const char*, license)

#define VINBERO_COM_MODULE_META_VERSION(major, minor, patch) \
VINBERO_COM_MODULE_META(VERSION, const char*, #major"."#minor"."#patch) \
VINBERO_COM_MODULE_META(VERSION_MAJOR, int, major) \
VINBERO_COM_MODULE_META(VERSION_MINOR, int, minor) \
VINBERO_COM_MODULE_META(VERSION_PATCH, int, patch)

#define VINBERO_COM_MODULE_META_IN_IFACES(ifaces) \
VINBERO_COM_MODULE_META(IN_IFACES, const char*, ifaces)

#define VINBERO_COM_MODULE_META_OUT_IFACES(ifaces) \
VINBERO_COM_MODULE_META(OUT_IFACES, const char*, ifaces)

/* -1,-1 for unlimited child count */
#define VINBERO_COM_MODULE_META_CHILD_COUNT(min, max) \
VINBERO_COM_MODULE_META(MIN_CHILD_COUNT, int, min) \
VINBERO_COM_MODULE_META(MAX_CHILD_COUNT, int, max)

#endif
