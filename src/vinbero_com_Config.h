#ifndef _VINBERO_COM_CONFIG_H
#define _VINBERO_COM_CONFIG_H

#include <jansson.h>
#include <string.h>
#include "vinbero_com_Object.h"

struct vinbero_com_Config {
    union {
        json_t* json;
        yaml_parser_t* yaml;
    };
    struct vinbero_com_Object* object;
};

#include "vinbero_com_Module.h"

int vinbero_com_Config_fromString(struct vinbero_com_Config* config, const char* input);
int vinbero_com_Config_fromFile(struct vinbero_com_Config* config, const char* path);
int vinbero_com_Config_fromJsonFile(struct vinbero_com_Config* config, const char* path);
int vinbero_com_Config_fromYamlFile(struct vinbero_com_Config* config, const char* path);
int vinbero_com_Config_check(struct vinbero_com_Config* config, const char* moduleId);

void vinbero_com_Config_getInt(struct vinbero_com_Config* config, struct vinbero_com_Module* module, const char* key, int* out, int defaultValue);
void vinbero_com_Config_getConstring(struct vinbero_com_Config* config, struct vinbero_com_Module* module, const char* key, const char** out, const char* defaultValue);
void vinbero_com_Config_getDouble(struct vinbero_com_Config* config, struct vinbero_com_Module* module, const char* key, double* out, double defaultValue);
void vinbero_com_Config_getBool(struct vinbero_com_Config* config, struct vinbero_com_Module* module, const char* key, bool* out, bool defaultValue);
bool vinbero_com_Config_getRequiredInt(struct vinbero_com_Config* config, struct vinbero_com_Module* module, const char* key, int* out);
bool vinbero_com_Config_getRequiredConstring(struct vinbero_com_Config* config, struct vinbero_com_Module* module, const char* key, const char** out);
bool vinbero_com_Config_getRequiredDouble(struct vinbero_com_Config* config, struct vinbero_com_Module* module, const char* key, double* out);
bool vinbero_com_Config_getRequiredBool(struct vinbero_com_Config* config, struct vinbero_com_Module* module, const char* key, bool* out);

int vinbero_com_Config_getModulePath(struct vinbero_com_Config* config, const char* moduleId, const char** modulePath);
size_t vinbero_com_Config_getChildModuleCount(struct vinbero_com_Config* config, const char* moduleId);
int vinbero_com_Config_getChildModuleIds(struct vinbero_com_Config* config, const char* moduleId, struct vinbero_com_Object** ids);
int vinbero_com_Config_init(struct vinbero_com_Config* config);
int vinbero_com_Config_destroy(struct vinbero_com_Config* config);

#define VINBERO_MODULE_CONFIG_GET(config, module, key, type, value) do { \
    *(value) = NULL; \
    GENC_MTREE_NODE_GET((config)->object, (module)->id, strlen((module)->id), value); \
    if(*(value) != NULL) { \
        GENC_MTREE_NODE_GET(*(value), "config", sizeof("config") - 1, value); \
        if(*(value) != NULL) { \
            GENC_MTREE_NODE_GET(*(value), key, strlen(key), value); \
            if(*(value) != NULL) { \
                if(VINBERO_COM_OBJECT_TYPE(*(value)) != VINBERO_COM_OBJECT_TYPE_##type) \
                    *(value) = NULL; \
            } \
        } \
    } \
    for(struct vinbero_com_Module* currentModule = module; \
        *(value) == NULL && GENC_TREE_NODE_PARENT(currentModule) != NULL; \
        currentModule = GENC_TREE_NODE_PARENT(currentModule)) { \
        GENC_MTREE_NODE_GET((config)->object, currentModule->id, strlen(currentModule->id), value); \
        if(*(value) == NULL) \
            continue; \
        GENC_MTREE_NODE_GET(*(value), "config", sizeof("config") - 1, value); \
        if(*(value) == NULL) \
            continue; \
        GENC_MTREE_NODE_GET(*(value), key, strlen(key), value); \
        if(*(value) == NULL) \
            continue; \
        if(VINBERO_COM_OBJECT_TYPE(*(value)) != VINBERO_COM_OBJECT_TYPE_##type) \
            *(value) = NULL; \
        break; \
    } \
} while(0)

#define VINBERO_MODULE_CONFIG_GET_REQ(config, module, key, type, value) do { \
    VINBERO_MODULE_CONFIG_GET(config, module, key, type, value); \
    if(*(value) == NULL) \
        VINBERO_COM_LOG_ERROR("REQUIRED CONFIG %s NOT FOUND ON MODULE %s", key, (module)->id); \
} while(0)

#define VINBERO_COM_CONFIG_MGET(config, module, key, type, value) do { \
    *(value) = NULL; \
    GENC_MTREE_NODE_GET((config)->object, (module)->id, strlen((module)->id), value); \
    if(*(value) == NULL) \
        break; \
    GENC_MTREE_NODE_GET(*(value), key, strlen(key), value); \
    if(*(value) == NULL) \
        break; \
    if(VINBERO_COM_OBJECT_TYPE(*(value)) != VINBERO_COM_OBJECT_TYPE_##type) { \
        *(value) = NULL; \
        break; \
    } \
} while(0)

#define VINBERO_COM_CONFIG_MGET_REQ(config, module, key, type, value) \
VINBERO_COM_CONFIG_MGET(config, module, key, type, value)

#endif
