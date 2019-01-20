#ifndef _VINBERO_COMMON_CONFIG_H
#define _VINBERO_COMMON_CONFIG_H

#include <jansson.h>
#include <string.h>
#include "vinbero_common_Object.h"

struct vinbero_common_Config {
    json_t* json;
    struct vinbero_common_Object* object;
};

#include "vinbero_common_Module.h"

int vinbero_common_Config_fromString(struct vinbero_common_Config* config, const char* input);
int vinbero_common_Config_fromFile(struct vinbero_common_Config* config, const char* path);
int vinbero_common_Config_check(struct vinbero_common_Config* config, const char* moduleId);

void vinbero_common_Config_getInt(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* key, int* out, int defaultValue);
void vinbero_common_Config_getConstring(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* key, const char** out, const char* defaultValue);
void vinbero_common_Config_getDouble(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* key, double* out, double defaultValue);
void vinbero_common_Config_getBool(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* key, bool* out, bool defaultValue);
bool vinbero_common_Config_getRequiredInt(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* key, int* out);
bool vinbero_common_Config_getRequiredConstring(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* key, const char** out);
bool vinbero_common_Config_getRequiredDouble(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* key, double* out);
bool vinbero_common_Config_getRequiredBool(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* key, bool* out);

int vinbero_common_Config_getModulePath(struct vinbero_common_Config* config, const char* moduleId, const char** modulePath);
size_t vinbero_common_Config_getChildModuleCount(struct vinbero_common_Config* config, const char* moduleId);
int vinbero_common_Config_getChildModuleIds(struct vinbero_common_Config* config, const char* moduleId, struct vinbero_common_Object** ids);
int vinbero_common_Config_init(struct vinbero_common_Config* config);
int vinbero_common_Config_destroy(struct vinbero_common_Config* config);

#define VINBERO_MODULE_CONFIG_GET(config, module, key, type, value) do { \
    for(struct vinbero_common_Module* currentModule = module; \
        GENC_TREE_NODE_PARENT(currentModule) != NULL; \
        currentModule = GENC_TREE_NODE_PARENT(currentModule)) { \
        GENC_MTREE_NODE_GET_CHILD((config)->object, currentModule->id, strlen(currentModule->id), value); \
        if(*(value) == NULL) \
            continue; \
        GENC_MTREE_NODE_GET_CHILD(*(value), "config", sizeof("config") - 1, value); \
        if(*(value) == NULL) \
            continue; \
        GENC_MTREE_NODE_GET_CHILD(*(value), key, strlen(key), value); \
        if(*(value) == NULL) \
            continue; \
        if(VINBERO_COMMON_OBJECT_TYPE(*(value)) != VINBERO_COMMON_OBJECT_TYPE_##type) \
            *(value) = NULL; \
        break; \
    } \
} while(0)

#define VINBERO_MODULE_CONFIG_GET_REQ(config, module, key, type, value) do { \
    VINBERO_MODULE_CONFIG_GET(config, module, key, type, value); \
    if(*(value) == NULL) \
        VINBERO_COMMON_LOG_ERROR("Required config value %s not found at module %s", key, (module)->id); \
} while(0)

#define VINBERO_COMMON_CONFIG_MGET(config, module, key, type, value) do { \
    GENC_MTREE_NODE_GET_CHILD((config)->object, (module)->id, strlen((module)->id), value); \
    if(*(value) == NULL) \
        break; \
    GENC_MTREE_NODE_GET_CHILD(*(value), key, strlen(key), value) \
    if(*(value) == NULL) \
        break; \
    if(VINBERO_COMMON_OBJECT_TYPE(*(value)) != VINBERO_COMMON_OBJECT_TYPE_##type) { \
        *(value) = NULL; \
        break; \
    } \
} while(0)

#define VINBERO_COMMON_CONFIG_MGET_REQ(config, module, key, type, value) \
VINBERO_COMMON_CONFIG_MGET(config, module, key, type, value)

#endif
