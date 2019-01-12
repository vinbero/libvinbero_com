#include <jansson.h>
#include <libgenc/genc_Tree.h>
#include <libgenc/genc_ArrayList.h>
#include "vinbero_common_Status.h"
#include "vinbero_common_Error.h"
#include "vinbero_common_Config.h"
#include "vinbero_common_Log.h"

int vinbero_common_Config_fromString(struct vinbero_common_Config* config, const char* input) {
    json_error_t configError;
    if(config->json == NULL) {
        if((config->json = json_loads(input, 0, &configError)) == NULL) {
            VINBERO_COMMON_LOG_ERROR("%s: %d: %s", configError.source, configError.line, configError.text);
            return VINBERO_COMMON_ERROR_INVALID_CONFIG;
        }
    } else {
        VINBERO_COMMON_LOG_ERROR("Config is already allocated");
        return VINBERO_COMMON_ERROR_ALREADY;
    }
    return VINBERO_COMMON_STATUS_SUCCESS;
}

int vinbero_common_Config_fromFile(struct vinbero_common_Config* config, const char* path) {
    json_error_t configError;
    if(config->json == NULL) {
        if((config->json = json_load_file(path, 0, &configError)) == NULL) {
            VINBERO_COMMON_LOG_ERROR("%s: %d: %s", configError.source, configError.line, configError.text);
            return VINBERO_COMMON_ERROR_INVALID_CONFIG;
        }
    } else {
        VINBERO_COMMON_LOG_ERROR("Config is already allocated");
        return VINBERO_COMMON_ERROR_ALREADY;
    }
    return VINBERO_COMMON_STATUS_SUCCESS;
}

int vinbero_common_Config_check(struct vinbero_common_Config* config, const char* moduleId) {
    json_t* moduleJson;
    json_t* moduleConfigJson;
    json_t* moduleChildrenJson;
    if((moduleJson = json_object_get((config)->json, moduleId)) != NULL
       && json_is_object(moduleJson)
       && (moduleConfigJson = json_object_get(moduleJson, "config")) != NULL
       && json_is_object(moduleConfigJson)
       && (moduleChildrenJson = json_object_get(moduleJson, "next")) != NULL
       && json_is_array(moduleChildrenJson)) {
        return VINBERO_COMMON_STATUS_SUCCESS;
    } else
        return VINBERO_COMMON_ERROR_INVALID_CONFIG;
}

#define VINBERO_MODULE_CONFIG_GET(config, module, valueName, valueType, out, defaultValue) do { \
    bool valueFound = false; \
    json_t* outJson; \
    for(struct vinbero_common_Module* currentModule = module; \
        GENC_TREE_NODE_PARENT(currentModule) != NULL; \
        currentModule = GENC_TREE_NODE_PARENT(currentModule)) { \
        if((outJson = json_object_get(json_object_get(json_object_get((config)->json, currentModule->id), "config"), valueName)) != NULL) { \
            *(out) = json_##valueType##_value(outJson); \
            valueFound = true; \
            break; \
        } \
    } \
    if(valueFound == false) \
        *(out) = defaultValue; \
} while(0)

void vinbero_common_Config_getInt(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, int* out, int defaultValue) {
    VINBERO_MODULE_CONFIG_GET(config, module, valueName, integer, out, defaultValue);
    VINBERO_COMMON_LOG_DEBUG("Config option %s: %d", valueName, *out);
}

void vinbero_common_Config_getString(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, const char** out, const char* defaultValue) {
    VINBERO_MODULE_CONFIG_GET(config, module, valueName, string, out, defaultValue);
    VINBERO_COMMON_LOG_DEBUG("Config option %s: %s", valueName, *out);
}

void vinbero_common_Config_getDouble(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, double* out, double defaultValue) {
    VINBERO_MODULE_CONFIG_GET(config, module, valueName, number, out, defaultValue);
    VINBERO_COMMON_LOG_DEBUG("Config option %s: %f", valueName, *out);
}

void vinbero_common_Config_getBool(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, bool* out, bool defaultValue) {
    VINBERO_MODULE_CONFIG_GET(config, module, valueName, boolean, out, defaultValue);
    VINBERO_COMMON_LOG_DEBUG("Config option %s: %s", valueName, *out == true ? "true" : "false");
}

#define VINBERO_MODULE_CONFIG_GET_REQ(config, module, valueName, valueType, out, ret) do { \
    *(ret) = VINBERO_COMMON_ERROR_INVALID_CONFIG; \
    json_t* outJson; \
    for(struct vinbero_common_Module* currentModule = module; \
        GENC_TREE_NODE_PARENT(currentModule) != NULL; \
        currentModule = GENC_TREE_NODE_PARENT(currentModule)) { \
        if((outJson = json_object_get(json_object_get(json_object_get((config)->json, currentModule->id), "config"), valueName)) != NULL) { \
            *(out) = json_##valueType##_value(outJson); \
            *(ret) = VINBERO_COMMON_STATUS_SUCCESS; \
            break; \
        } \
    } \
    if(*(ret) != VINBERO_COMMON_STATUS_SUCCESS) \
        VINBERO_COMMON_LOG_ERROR("Config value %s not found at module %s", valueName, module->id); \
} while(0)

int vinbero_common_Config_getRequiredInt(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, int* out) {
    int ret;
    VINBERO_MODULE_CONFIG_GET_REQ(config, module, valueName, integer, out, &ret);
    if(ret == VINBERO_COMMON_STATUS_SUCCESS)
        VINBERO_COMMON_LOG_DEBUG("Config option %s: %d", valueName, *out);
    return ret;
}

int vinbero_common_Config_getRequiredString(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, const char** out) {
    int ret;
    VINBERO_MODULE_CONFIG_GET_REQ(config, module, valueName, string, out, &ret);
    if(ret == VINBERO_COMMON_STATUS_SUCCESS)
        VINBERO_COMMON_LOG_DEBUG("Config option %s: %s", valueName, *out);
    return ret;
}

int vinbero_common_Config_getRequiredDouble(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, double* out) {
    int ret;
    VINBERO_MODULE_CONFIG_GET_REQ(config, module, valueName, number, out, &ret);
    if(ret == VINBERO_COMMON_STATUS_SUCCESS)
        VINBERO_COMMON_LOG_DEBUG("Config option %s: %f", valueName, *out);
    return ret;
}

int vinbero_common_Config_getRequiredBool(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* valueName, bool* out) {
    int ret;
    VINBERO_MODULE_CONFIG_GET_REQ(config, module, valueName, number, out, &ret);
    if(ret == VINBERO_COMMON_STATUS_SUCCESS)
        VINBERO_COMMON_LOG_DEBUG("Config option %s: %s", valueName, *out == true ? "true" : "false");
    return ret;
}

size_t vinbero_common_Config_getChildModuleCount(struct vinbero_common_Config* config, const char* moduleId) {
    json_t* array = json_object_get(json_object_get((config)->json, moduleId), "next");
    return json_array_size(array);
}

int vinbero_common_Config_getChildModuleIds(struct vinbero_common_Config* config, const char* moduleId, struct vinbero_common_Module_Ids* out) {
    size_t childModuleCount;
    childModuleCount = vinbero_common_Config_getChildModuleCount(config, moduleId);
    json_t* childModuleIdsJson = json_object_get(json_object_get((config)->json, moduleId), "next");
    GENC_ARRAY_LIST_REALLOC(out, childModuleCount);
    json_t* childModuleIdJson;
    struct vinbero_common_Module_Id* id;
    size_t index;
    json_array_foreach(childModuleIdsJson, index, childModuleIdJson) {
        id = malloc(sizeof(struct vinbero_common_Module_Id));
        id->value = json_string_value(childModuleIdJson);
        GENC_ARRAY_LIST_PUSH(out, id);
    }
    return VINBERO_COMMON_STATUS_SUCCESS;
}

int vinbero_common_Config_init(struct vinbero_common_Config* config) {
    config->json = NULL;
    return VINBERO_COMMON_STATUS_SUCCESS;
}

int vinbero_common_Config_destroy(struct vinbero_common_Config* config) {
    json_decref((config)->json);
    return VINBERO_COMMON_STATUS_SUCCESS;
}
