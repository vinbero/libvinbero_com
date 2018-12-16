#ifndef _VINBERO_COMMON_CONFIG_H
#define _VINBERO_COMMON_CONFIG_H

#include <jansson.h>

struct vinbero_common_Config {
    json_t* json;
};

#include "vinbero_common_Module.h"

int vinbero_common_Config_fromString(struct vinbero_common_Config* config, const char* input);
int vinbero_common_Config_fromFile(struct vinbero_common_Config* config, const char* path);
int vinbero_common_Config_check(struct vinbero_common_Config* config, const char* moduleId);
void vinbero_common_Config_getInt(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, int* out, int defaultValue);
void vinbero_common_Config_getString(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, const char** out, const char* defaultValue);
void vinbero_common_Config_getDouble(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, double* out, double defaultValue);
void vinbero_common_Config_getBool(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, bool* out, bool defaultValue);
int vinbero_common_Config_getRequiredInt(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, int* out);
int vinbero_common_Config_getRequiredString(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, const char** out);
int vinbero_common_Config_getRequiredDouble(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, double* out);
int vinbero_common_Config_getRequiredBool(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, bool* out);
int vinbero_common_Config_getModulePath(struct vinbero_common_Config* config, const char* moduleId, const char** modulePath);
size_t vinbero_common_Config_getChildModuleCount(struct vinbero_common_Config* config, const char* moduleId);
int vinbero_common_Config_getChildModuleIds(struct vinbero_common_Config* config, const char* moduleId, struct vinbero_common_Module_Ids* out);
int vinbero_common_Config_init(struct vinbero_common_Config* config);
int vinbero_common_Config_destroy(struct vinbero_common_Config* config);

#define VINBERO_COMMON_CONFIG_MGET_REQ(config, module, key, type, value, ret) \
do { \
    *(value) = json_##type##_value(json_object_get(json_object_get((config)->json, (module)->id), key)); \
    if(*(value) == NULL) \
        *(ret) = VINBERO_COMMON_ERROR_NOT_FOUND; \
    *(ret) = VINBERO_COMMON_STATUS_SUCCESS; \
} while(0)

#endif
