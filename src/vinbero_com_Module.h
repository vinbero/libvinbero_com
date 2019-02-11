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

#define VINBERO_COM_MODULE_DLSYM(iface, dlHandle, funcName, ret) do { \
    (iface)->funcName = NULL; \
    VINBERO_COM_DLSYM(dlHandle, #funcName, (void**)&(iface)->funcName, ret); \
} while(0)

#define VINBERO_COM_MODULE_META(name, type, value) \
type vinbero_com_Module_Metadata_##name() { \
    return value; \
}

#define VINBERO_COM_MODULE_META_GET(dlHandle, name, type, value, ret) do { \
  type (*vinbero_com_Module_Metadata_##name)() meta; \
  VINBERO_COM_DLSYM(dlHandle, "vinbero_com_Module_Metadata_"#name, ret) \
  if(ret < VINBERO_COM_STATUS_SUCCESS) \
    break; \
  *(value) = meta(); \
} while(0)

#define VINBERO_COM_MODULE_META_NAME(name) \
VINBERO_COM_MODULE_META(NAME, const char*, name)

#define VINBERO_COM_MODULE_META_VERSION(major, minor, patch) \
VINBERO_COM_MODULE_META(VERSION, const char*, major"."minor"."patch) \
VINBERO_COM_MODULE_META(VERSION_MAJOR, const char*, major) \
VINBERO_COM_MODULE_META(VERSION_MINOR, const char*, minor) \
VINBERO_COM_MODULE_META(VERSION_PATCH, const char*, minor)

#define VINBERO_COM_MODULE_META_IN_IFACES(ifaces) \
VINBERO_COM_MODULE_META(IN_IFACES, const char*, ifaces)

#define VINBERO_COM_MODULE_META_OUT_IFACES(ifaces) \
VINBERO_COM_MODULE_META(OUT_IFACES, const char*, ifaces)

#define VINBERO_COM_MODULE_META_INIT(name, major, minor, patch, inIfaces, outIfaces) \
VINBERO_COM_MODULE_META_NAME(name) \
VINBERO_COM_MODULE_META_VERSION(major, minor, patch) \
VINBERO_COM_MODULE_META_IN_IFACES(inIfaces) \
VINBERO_COM_MODULE_META_OUT_IFACES(outIfaces)

#endif
