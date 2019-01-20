#include <jansson.h>
#include <string.h>
#include <libgenc/genc_Mtree.h>
#include <libgenc/genc_Tree.h>
#include <libgenc/genc_ArrayList.h>
#include "vinbero_common_Status.h"
#include "vinbero_common_Error.h"
#include "vinbero_common_Config.h"
#include "vinbero_common_Log.h"

int vinbero_common_Config_fromString(struct vinbero_common_Config* config, const char* input) {
    json_error_t configError;
    if(config->object == NULL) {
        if((config->json = json_loads(input, 0, &configError)) == NULL) {
            VINBERO_COMMON_LOG_ERROR("%s: %d: %s", configError.source, configError.line, configError.text);
            return VINBERO_COMMON_ERROR_INVALID_CONFIG;
        }
    } else {
        VINBERO_COMMON_LOG_ERROR("Config is already allocated");
        return VINBERO_COMMON_ERROR_ALREADY;
    }
    config->object = vinbero_common_Object_fromJson(config->json);
    return VINBERO_COMMON_STATUS_SUCCESS;
}

int vinbero_common_Config_fromFile(struct vinbero_common_Config* config, const char* path) {
    json_error_t configError;
    if(config->object == NULL) {
        if((config->json = json_load_file(path, 0, &configError)) == NULL) {
            VINBERO_COMMON_LOG_ERROR("%s: %d: %s", configError.source, configError.line, configError.text);
            return VINBERO_COMMON_ERROR_INVALID_CONFIG;
        }
    } else {
        VINBERO_COMMON_LOG_ERROR("Config is already allocated");
        return VINBERO_COMMON_ERROR_ALREADY;
    }
    config->object = vinbero_common_Object_fromJson(config->json);
    return VINBERO_COMMON_STATUS_SUCCESS;
}

int vinbero_common_Config_check(struct vinbero_common_Config* config, const char* moduleId) {
    struct vinbero_common_Object* object;
    struct vinbero_common_Object* object2;

    GENC_MTREE_NODE_GET_CHILD(config->object, moduleId, strlen(moduleId), &object);
    if(object == NULL)
        return VINBERO_COMMON_ERROR_NOT_FOUND;
    if(VINBERO_COMMON_OBJECT_TYPE(object) != VINBERO_COMMON_OBJECT_TYPE_MAP)
        return VINBERO_COMMON_ERROR_INVALID_CONFIG;
    GENC_MTREE_NODE_GET_CHILD(object, "config", sizeof("config") - 1, &object2);
    if(object2 == NULL || VINBERO_COMMON_OBJECT_TYPE(object2) != VINBERO_COMMON_OBJECT_TYPE_MAP)
        return VINBERO_COMMON_ERROR_INVALID_CONFIG;
    GENC_MTREE_NODE_GET_CHILD(object, "next", sizeof("next") - 1, &object2);
    if(object2 == NULL || VINBERO_COMMON_OBJECT_TYPE(object2) != VINBERO_COMMON_OBJECT_TYPE_ARRAY)
        return VINBERO_COMMON_ERROR_INVALID_CONFIG;
    return VINBERO_COMMON_STATUS_SUCCESS;
}

#define VINBERO_MODULE_CONFIG_GET(config, module, key, type, value) do { \
    bool valueFound = false; \
    struct vinbero_common_Object* tmp; \
    for(struct vinbero_common_Module* currentModule = module; \
        GENC_TREE_NODE_PARENT(currentModule) != NULL; \
        currentModule = GENC_TREE_NODE_PARENT(currentModule)) { \
        GENC_MTREE_NODE_GET_CHILD((config)->object, currentModule->id, strlen(currentModule->id), &tmp); \
        if(tmp == NULL) \
            continue; \
        GENC_MTREE_NODE_GET_CHILD(tmp, "config", sizeof("config") - 1, &tmp); \
        if(tmp == NULL) \
            continue; \
        GENC_MTREE_NODE_GET_CHILD(tmp, key, strlen(key), &tmp); \
        if(tmp == NULL) \
            continue; \
        if(VINBERO_COMMON_OBJECT_TYPE(tmp) != VINBERO_COMMON_OBJECT_TYPE_##type) \
            break; \
        *(value) = tmp; \
        valueFound = true; \
        break; \
    } \
    if(valueFound == false) { \
        *(value) = NULL; \
        VINBERO_COMMON_LOG_ERROR("Config value %s not found at module %s", key, (module)->id); \
    } \
} while(0)

#define VINBERO_MODULE_CONFIG_GET_REQ(config, module, key, type, value) do { \
    VINBERO_MODULE_CONFIG_GET(config, module, key, type, value); \
    if(*(value) == NULL) \
        VINBERO_COMMON_LOG_ERROR("Config value %s not found at module %s", key, (module)->id); \
} while(0)

void vinbero_common_Config_getInt(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* key, int* value, int defaultValue) {
    struct vinbero_common_Object* object;
    VINBERO_MODULE_CONFIG_GET(config, module, key, INTEGER, &object);
    if(object == NULL)
        *value = defaultValue;
    else
        *value = VINBERO_COMMON_OBJECT_INTEGER(object);
    VINBERO_COMMON_LOG_DEBUG("Config option %s: %d", key, *value);
}

void vinbero_common_Config_getConstring(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* key, const char** value, const char* defaultValue) {
    struct vinbero_common_Object* object;
    VINBERO_MODULE_CONFIG_GET(config, module, key, CONSTRING, &object);
    if(object == NULL)
        *value = defaultValue;
    else
        *value = VINBERO_COMMON_OBJECT_CONSTRING(object);
    VINBERO_COMMON_LOG_DEBUG("Config option %s: %s", key, *value);
}

void vinbero_common_Config_getDouble(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* key, double* value, double defaultValue) {
    struct vinbero_common_Object* object;
    VINBERO_MODULE_CONFIG_GET(config, module, key, REAL, &object);
    if(object == NULL)
        *value = defaultValue;
    else
        *value = VINBERO_COMMON_OBJECT_REAL(object);
    VINBERO_COMMON_LOG_DEBUG("Config option %s: %f", key, *value);
}

void vinbero_common_Config_getBool(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* key, bool* value, bool defaultValue) {
    struct vinbero_common_Object* object;
    VINBERO_MODULE_CONFIG_GET(config, module, key, BOOLEAN, &object);
    if(object == NULL)
        *value = defaultValue;
    else
        *value = VINBERO_COMMON_OBJECT_BOOLEAN(object);
    VINBERO_COMMON_LOG_DEBUG("Config option %s: %s", key, *value == true ? "true" : "false");
}

bool vinbero_common_Config_getRequiredInt(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* key, int* value) {
    struct vinbero_common_Object* object;
    VINBERO_MODULE_CONFIG_GET_REQ(config, module, key, INTEGER, &object);
    if(object == NULL)
        return false;
    else {
        *value = VINBERO_COMMON_OBJECT_INTEGER(object);
        VINBERO_COMMON_LOG_DEBUG("Config option %s: %d", key, *value);
        return true;
    }
}

bool vinbero_common_Config_getRequiredConstring(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* key, const char** value) {
    struct vinbero_common_Object* object;
    VINBERO_MODULE_CONFIG_GET_REQ(config, module, key, CONSTRING, &object);
    if(object == NULL)
        return false;
    else {
        *value = VINBERO_COMMON_OBJECT_CONSTRING(object);
        VINBERO_COMMON_LOG_DEBUG("Config option %s: %s", key, *value);
        return true;
    }
}

bool vinbero_common_Config_getRequiredDouble(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* key, double* value) {
    struct vinbero_common_Object* object;
    VINBERO_MODULE_CONFIG_GET_REQ(config, module, key, CONSTRING, &object);
    if(object == NULL)
        return false;
    else {
        *value = VINBERO_COMMON_OBJECT_REAL(object);
        VINBERO_COMMON_LOG_DEBUG("Config option %s: %f", key, *value);
        return true;
    }
}

bool vinbero_common_Config_getRequiredBool(struct vinbero_common_Config* config, struct vinbero_common_Module* module, const char* key, bool* value) {
    struct vinbero_common_Object* object;
    VINBERO_MODULE_CONFIG_GET_REQ(config, module, key, CONSTRING, &object);
    if(object == NULL)
        return false;
    else {
        *value = VINBERO_COMMON_OBJECT_BOOLEAN(object);
        VINBERO_COMMON_LOG_DEBUG("Config option %s: %s", key, *value == true ? "true" : "false");
        return true;
    }
}

size_t vinbero_common_Config_getChildModuleCount(struct vinbero_common_Config* config, const char* moduleId) {
    struct vinbero_common_Object* object;
    GENC_MTREE_NODE_GET_CHILD(config->object, moduleId, strlen(moduleId), &object)
    GENC_MTREE_NODE_GET_CHILD(object, "next", sizeof("next") - 1, &object);
    return GENC_TREE_NODE_CHILD_COUNT(object);
}

int vinbero_common_Config_getChildModuleIds(struct vinbero_common_Config* config, const char* moduleId, struct vinbero_common_Object** ids) {
    struct vinbero_common_Object* tmp;
    GENC_MTREE_NODE_GET_CHILD(config->object, moduleId, strlen(moduleId), &tmp)
    if(tmp == NULL)
        return VINBERO_COMMON_ERROR_NOT_FOUND;
    printf("module config found\n");
    GENC_MTREE_NODE_GET_CHILD(tmp, "next", sizeof("next") - 1, ids);

    if(*ids == NULL)
        return VINBERO_COMMON_ERROR_NOT_FOUND;
    printf("next config found\n");
    return VINBERO_COMMON_STATUS_SUCCESS;
}

int vinbero_common_Config_init(struct vinbero_common_Config* config) {
    config->json = NULL;
    config->object = NULL;
    return VINBERO_COMMON_STATUS_SUCCESS;
}

int vinbero_common_Config_destroy(struct vinbero_common_Config* config) {
    if(config->object != NULL) {
        vinbero_common_Object_destroy(config->object);
    }
    if(config->json != NULL)
        json_decref(config->json);
    return VINBERO_COMMON_STATUS_SUCCESS;
}
