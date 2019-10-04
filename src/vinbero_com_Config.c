#include <jansson.h>
#include <string.h>
#include <libgenc/genc_Mtree.h>
#include <libgenc/genc_Tree.h>
#include "vinbero_com_Status.h"
#include "vinbero_com_Error.h"
#include "vinbero_com_Config.h"
#include "vinbero_com_Log.h"

enum vinbero_com_Config_Type {
    VINBERO_COM_CONFIG_TYPE_JSON,
    VINBERO_COM_CONFIG_TYPE_YAML,
    VINBERO_COM_CONFIG_TYPE_TOML,
    VINBERO_COM_CONFIG_TYPE_UNKNOWN
};

int vinbero_com_Config_fromString(struct vinbero_com_Config* config, const char* input) {
    json_error_t configError;
    if(config->object == NULL) {
        if((config->json = json_loads(input, 0, &configError)) == NULL) {
            VINBERO_COM_LOG_ERROR("JSON PARSING ERROR %s: %d: %s", configError.source, configError.line, configError.text);
            return VINBERO_COM_ERROR_INVALID_CONFIG;
        }
    } else {
        VINBERO_COM_LOG_ERROR("CONFIG IS ALREADY ALLOCATED");
        return VINBERO_COM_ERROR_ALREADY;
    }
    config->object = vinbero_com_Object_fromJson(config->json);
    return VINBERO_COM_STATUS_SUCCESS;
}

static const char* vinbero_com_Config_getExtPoint(const char* path) {
    int len = strlen(path);
    for(int i = len - 1; i > 0; --i) {
        if(path[i] == '.')
            return path + i + 1;
    }
    return NULL;
}

static enum vinbero_com_Config_Type vinbero_com_Config_getFileType(const char* path) {
    // check the extension
    // json, yaml
    // TODO: toml
    // if ext not found => error

    static const char* confExt[4] = { "yaml", "yml", "json", "toml" };
    static const enum vinbero_com_Config_Type types[4] = { 
        VINBERO_COM_CONFIG_TYPE_YAML,
        VINBERO_COM_CONFIG_TYPE_YAML,
        VINBERO_COM_CONFIG_TYPE_JSON,
        VINBERO_COM_CONFIG_TYPE_TOML
    };

    for(int i = 0; i < 4; ++i) {
        const char* extPoint = vinbero_com_Config_getExtPoint(path);
        if(extPoint != NULL && strcasecmp(extPoint, confExt[i]) == 0)
            return types[i]; 
    }
    return VINBERO_COM_CONFIG_TYPE_UNKNOWN;
}

int vinbero_com_Config_fromFile(struct vinbero_com_Config* config, const char* path) {
    switch ( vinbero_com_Config_getFileType(path) ) {
        case VINBERO_COM_CONFIG_TYPE_JSON:
            return vinbero_com_Config_fromJsonFile(config, path);
        case VINBERO_COM_CONFIG_TYPE_TOML:
        case VINBERO_COM_CONFIG_TYPE_YAML:
            //return vinbero_com_Config_fromYamlFile(config, path);
        default:
            return VINBERO_COM_ERROR_INVALID_CONFIG;
    }
}

int vinbero_com_Config_fromJsonFile(struct vinbero_com_Config* config, const char* path) {
    json_error_t configError;
    if(config->object == NULL) {
        if((config->json = json_load_file(path, 0, &configError)) == NULL) {
            VINBERO_COM_LOG_ERROR("json error: %s: %d: %s", configError.source, configError.line, configError.text);
            return VINBERO_COM_ERROR_INVALID_CONFIG;
        }
    } else {
        VINBERO_COM_LOG_ERROR("CONFIG IS ALREADY ALLOCATED");
        return VINBERO_COM_ERROR_ALREADY;
    }
    config->object = vinbero_com_Object_fromJson(config->json);
    return VINBERO_COM_STATUS_SUCCESS;
}

int vinbero_com_Config_fromYamlFile(struct vinbero_com_Config* config, const char* path) {
    FILE* yamlFile = fopen(path, "r");
    if (!yamlFile) return VINBERO_COM_ERROR_UNKNOWN;

    yaml_parser_t* parser = (yaml_parser_t*)malloc(sizeof(yaml_parser_t));
    if( !yaml_parser_initialize(parser)) 
        return VINBERO_COM_ERROR_UNKNOWN;
    
    yaml_parser_set_input_file(parser, yamlFile);
    config->yaml = parser;
    if (!config->object)
        config->object = vinbero_com_Object_fromYaml(config->yaml);
    else return VINBERO_COM_ERROR_ALREADY;

    return VINBERO_COM_STATUS_SUCCESS;
}

int vinbero_com_Config_check(struct vinbero_com_Config* config, const char* moduleId) {
    struct vinbero_com_Object* object;
    struct vinbero_com_Object* object2;

    GENC_MTREE_NODE_GET(config->object, moduleId, strlen(moduleId), &object);
    if(object == NULL)
        return VINBERO_COM_ERROR_NOT_FOUND;
    if(VINBERO_COM_OBJECT_TYPE(object) != VINBERO_COM_OBJECT_TYPE_MAP)
        return VINBERO_COM_ERROR_INVALID_CONFIG;
    GENC_MTREE_NODE_GET(object, "config", sizeof("config") - 1, &object2);
    if(object2 == NULL || VINBERO_COM_OBJECT_TYPE(object2) != VINBERO_COM_OBJECT_TYPE_MAP)
        return VINBERO_COM_ERROR_INVALID_CONFIG;
    GENC_MTREE_NODE_GET(object, "next", sizeof("next") - 1, &object2);
    if(object2 == NULL || VINBERO_COM_OBJECT_TYPE(object2) != VINBERO_COM_OBJECT_TYPE_ARRAY)
        return VINBERO_COM_ERROR_INVALID_CONFIG;
    return VINBERO_COM_STATUS_SUCCESS;
}

void vinbero_com_Config_getInt(struct vinbero_com_Config* config, struct vinbero_com_Module* module, const char* key, int* value, int defaultValue) {
    struct vinbero_com_Object* object;
    VINBERO_COM_MODULE_CONFIG_GET(config, module, key, INTEGER, &object);
    if(object == NULL)
        *value = defaultValue;
    else
        *value = VINBERO_COM_OBJECT_INTEGER(object);
    VINBERO_COM_LOG_DEBUG("CONFIG VALUE %s: %d", key, *value);
}

void vinbero_com_Config_getConstring(struct vinbero_com_Config* config, struct vinbero_com_Module* module, const char* key, const char** value, const char* defaultValue) {
    struct vinbero_com_Object* object;
    VINBERO_COM_MODULE_CONFIG_GET(config, module, key, CONSTRING, &object);
    if(object == NULL)
        *value = defaultValue;
    else
        *value = VINBERO_COM_OBJECT_CONSTRING(object);
    VINBERO_COM_LOG_DEBUG("CONFIG VALUE %s: %s", key, *value);
}

void vinbero_com_Config_getDouble(struct vinbero_com_Config* config, struct vinbero_com_Module* module, const char* key, double* value, double defaultValue) {
    struct vinbero_com_Object* object;
    VINBERO_COM_MODULE_CONFIG_GET(config, module, key, REAL, &object);
    if(object == NULL)
        *value = defaultValue;
    else
        *value = VINBERO_COM_OBJECT_REAL(object);
    VINBERO_COM_LOG_DEBUG("CONFIG VALUE %s: %f", key, *value);
}

void vinbero_com_Config_getBool(struct vinbero_com_Config* config, struct vinbero_com_Module* module, const char* key, bool* value, bool defaultValue) {
    struct vinbero_com_Object* object;
    VINBERO_COM_MODULE_CONFIG_GET(config, module, key, BOOLEAN, &object);
    if(object == NULL)
        *value = defaultValue;
    else
        *value = VINBERO_COM_OBJECT_BOOLEAN(object);
    VINBERO_COM_LOG_DEBUG("CONFIG VALUE %s: %s", key, *value == true ? "true" : "false");
}

bool vinbero_com_Config_getRequiredInt(struct vinbero_com_Config* config, struct vinbero_com_Module* module, const char* key, int* value) {
    struct vinbero_com_Object* object;
    VINBERO_COM_MODULE_CONFIG_GET_REQ(config, module, key, INTEGER, &object);
    if(object == NULL)
        return false;
    else {
        *value = VINBERO_COM_OBJECT_INTEGER(object);
        VINBERO_COM_LOG_DEBUG("CONFIG VALUE %s: %d", key, *value);
        return true;
    }
}

bool vinbero_com_Config_getRequiredConstring(struct vinbero_com_Config* config, struct vinbero_com_Module* module, const char* key, const char** value) {
    struct vinbero_com_Object* object;
    VINBERO_COM_MODULE_CONFIG_GET_REQ(config, module, key, CONSTRING, &object);
    if(object == NULL)
        return false;
    else {
        *value = VINBERO_COM_OBJECT_CONSTRING(object);
        VINBERO_COM_LOG_DEBUG("CONFIG VALUE %s: %s", key, *value);
        return true;
    }
}

bool vinbero_com_Config_getRequiredDouble(struct vinbero_com_Config* config, struct vinbero_com_Module* module, const char* key, double* value) {
    struct vinbero_com_Object* object;
    VINBERO_COM_MODULE_CONFIG_GET_REQ(config, module, key, CONSTRING, &object);
    if(object == NULL)
        return false;
    else {
        *value = VINBERO_COM_OBJECT_REAL(object);
        VINBERO_COM_LOG_DEBUG("CONFIG VALUE %s: %f", key, *value);
        return true;
    }
}

bool vinbero_com_Config_getRequiredBool(struct vinbero_com_Config* config, struct vinbero_com_Module* module, const char* key, bool* value) {
    struct vinbero_com_Object* object;
    VINBERO_COM_MODULE_CONFIG_GET_REQ(config, module, key, CONSTRING, &object);
    if(object == NULL)
        return false;
    else {
        *value = VINBERO_COM_OBJECT_BOOLEAN(object);
        VINBERO_COM_LOG_DEBUG("CONFIG VALUE %s: %s", key, *value == true ? "true" : "false");
        return true;
    }
}

size_t vinbero_com_Config_getChildModuleCount(struct vinbero_com_Config* config, const char* moduleId) {
    struct vinbero_com_Object* object;
    GENC_MTREE_NODE_GET(config->object, moduleId, strlen(moduleId), &object);
    GENC_MTREE_NODE_GET(object, "next", sizeof("next") - 1, &object);
    return GENC_TREE_NODE_SIZE(object);
}

int vinbero_com_Config_getChildModuleIds(struct vinbero_com_Config* config, const char* moduleId, struct vinbero_com_Object** ids) {
    struct vinbero_com_Object* tmp;
    GENC_MTREE_NODE_GET(config->object, moduleId, strlen(moduleId), &tmp);
    if(tmp == NULL)
        return VINBERO_COM_ERROR_NOT_FOUND;
    GENC_MTREE_NODE_GET(tmp, "next", sizeof("next") - 1, ids);
    if(*ids == NULL)
        return VINBERO_COM_ERROR_NOT_FOUND;
    return VINBERO_COM_STATUS_SUCCESS;
}

int vinbero_com_Config_init(struct vinbero_com_Config* config) {
    config->json = NULL;
    config->object = NULL;
    return VINBERO_COM_STATUS_SUCCESS;
}

int vinbero_com_Config_destroy(struct vinbero_com_Config* config) {
    if(config->object != NULL) {
        vinbero_com_Object_destroy(config->object);
    }
    // TODO: add yaml file destruction
    if(config->json != NULL)
        json_decref(config->json);
    return VINBERO_COM_STATUS_SUCCESS;
}
