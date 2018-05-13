#ifndef _VINBERO_COMMON_CONFIG_H
#define _VINBERO_COMMON_CONFIG_H

#include <jansson.h>

struct vinbero_common_Config {
    json_t* json;
};

#include "vinbero_common_Module.h"

int vinbero_common_Config_check(struct vinbero_common_Config* config, const char* moduleId);
int vinbero_common_Config_getInt(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, int* out, int defaultValue);
int vinbero_common_Config_getString(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, const char** out, const char* defaultValue);
int vinbero_common_Config_getDouble(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, double* out, double defaultValue);
int vinbero_common_Config_getRequiredInt(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, int* out);
int vinbero_common_Config_getRequiredString(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, const char** out);
int vinbero_common_Config_getRequiredDouble(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, double* out);
int vinbero_common_Config_getModulePath(struct vinbero_common_Config* config, const char* moduleId, const char** modulePath);
size_t vinbero_common_Config_getChildModuleCount(struct vinbero_common_Config* config, const char* moduleId);
int vinbero_common_Config_getChildModuleIds(struct vinbero_common_Config* config, const char* moduleId, struct vinbero_common_Module_Ids* out);
int vinbero_common_Config_destroy(struct vinbero_common_Config* config);

#endif
