#include <jansson.h>
#include <libgenc/genc_Tree.h>
#include <libgenc/genc_Mtree.h>
#include <yaml.h>
#include "vinbero_com_Object.h"

void vinbero_com_Object_destroy(struct vinbero_com_Object* object) {
    switch(VINBERO_COM_OBJECT_TYPE(object)) {
    case VINBERO_COM_OBJECT_TYPE_ARRAY:
        GENC_TREE_NODE_FOREACH(object, index) {
            vinbero_com_Object_destroy(GENC_TREE_NODE_RAW_GET(object, index));
        }
        GENC_TREE_NODE_FREE(object);
        break;
    case VINBERO_COM_OBJECT_TYPE_MAP:
        GENC_MTREE_NODE_FOREACH_BEGIN(object, childObject)
            vinbero_com_Object_destroy(childObject);
        GENC_MTREE_NODE_FOREACH_END(object, childObject);
        GENC_MTREE_NODE_FREE(object);
        break;
    default:
        break;
    }
    free(object);
}

struct vinbero_com_Object* vinbero_com_Object_fromJson(json_t* json) {
    struct vinbero_com_Object* object = malloc(sizeof(struct vinbero_com_Object));
    struct vinbero_com_Object* childObject;
    struct vinbero_com_Object* oldObject;
    size_t index;
    const char* key;
    json_t* value;
    switch(json_typeof(json)) {
    case JSON_NULL:
        VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_NULL);
        break;
    case JSON_FALSE:
        VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_BOOLEAN);
        VINBERO_COM_OBJECT_BOOLEAN(object) = false;
        break;
    case JSON_TRUE:
        VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_BOOLEAN);
        VINBERO_COM_OBJECT_BOOLEAN(object) = true;
        break;
    case JSON_INTEGER:
        VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_INTEGER);
        VINBERO_COM_OBJECT_INTEGER(object) = json_integer_value(json);
        break;
    case JSON_REAL:
        VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_REAL);
        VINBERO_COM_OBJECT_REAL(object) = json_real_value(json);
        break;
    case JSON_STRING:
        VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_CONSTRING);
        VINBERO_COM_OBJECT_CONSTRING(object) = json_string_value(json);
        break;
    case JSON_ARRAY:
        VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_ARRAY);
        json_array_foreach(json, index, value) {
            childObject = vinbero_com_Object_fromJson(value);
            if(childObject == NULL) {
                vinbero_com_Object_destroy(object);
                object = NULL;
            }
            GENC_TREE_NODE_ADD(object, childObject);
        }
        break;
    case JSON_OBJECT:
        VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_MAP);
        json_object_foreach(json, key, value) {
            childObject = vinbero_com_Object_fromJson(value);
            if(childObject == NULL) {
                vinbero_com_Object_destroy(object);
                object = NULL;
            }
            GENC_MTREE_NODE_KEY(childObject) = key;
            GENC_MTREE_NODE_KEY_LENGTH(childObject) = strlen(key);
            GENC_MTREE_NODE_SET(object, childObject, &oldObject);
        }
        break;
    default:
        free(object);
        object = NULL;
    }
    return object;
}

/*
 * parsing object recursive from yaml file object
 * You must free return value after use. Use vinbero_com_Object_destroy() function.
 * 
 * @param[IN]   parser    yaml parser object
 * 
*/
struct vinbero_com_Object* vinbero_com_Object_fromYaml(yaml_parser_t* parser)
{
    struct vinbero_com_Object* object;
    struct vinbero_com_Object* keyObject;
    struct vinbero_com_Object* childObject;
    struct vinbero_com_Object* oldObject;
    yaml_token_t token;
    for(yaml_parser_scan(parser, &token);
        token.type != YAML_STREAM_END_TOKEN;
        yaml_parser_scan(parser, &token)) {
        switch(token.type) {
        case YAML_NO_TOKEN:
            break;
        case YAML_STREAM_START_TOKEN:
            object = malloc(sizeof(struct vinbero_com_Object));
            VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_ARRAY);
            return object;
        case YAML_VERSION_DIRECTIVE_TOKEN:
            break;
        case YAML_TAG_DIRECTIVE_TOKEN:
            break;
        case YAML_DOCUMENT_START_TOKEN:
            break;
        case YAML_DOCUMENT_END_TOKEN:
            break;
        case YAML_BLOCK_SEQUENCE_START_TOKEN:
            object = malloc(sizeof(struct vinbero_com_Object));
            VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_ARRAY);
            for(; token.type != YAML_BLOCK_END_TOKEN; yaml_parser_scan(parser, &token)) {
                childObject = vinbero_com_Object_fromYaml(parser);
                GENC_TREE_NODE_ADD(object, childObject);
            }
            return object;
        case YAML_BLOCK_MAPPING_START_TOKEN:
            object = malloc(sizeof(struct vinbero_com_Object));
            VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_MAP);
            for(; token.type != YAML_BLOCK_END_TOKEN; yaml_parser_scan(parser, &token)) {
                switch(token.type) {
                case YAML_KEY_TOKEN:
                    keyObject = vinbero_com_Object_fromYaml(parser);
                    break;
                case YAML_VALUE_TOKEN:
                    childObject = vinbero_com_Object_fromYaml(parser);
                    GENC_MTREE_NODE_KEY(childObject) = keyObject;
                    GENC_MTREE_NODE_KEY_LENGTH(childObject) = sizeof(struct vinbero_com_Object);

                    GENC_MTREE_NODE_SET(object, childObject, &oldObject);
                    if(oldObject != NULL)
                        vinbero_com_Object_destroy(oldObject);
                    break;
                }
            }
            return object;
        case YAML_BLOCK_END_TOKEN:
            break;
        case YAML_FLOW_SEQUENCE_START_TOKEN:
            object = malloc(sizeof(struct vinbero_com_Object));
            VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_ARRAY);
            for(; token.type != YAML_FLOW_SEQUENCE_END_TOKEN; yaml_parser_scan(parser, &token)) {
                childObject = vinbero_com_Object_fromYaml(parser);
                GENC_TREE_NODE_ADD(object, childObject);
            }
            return object;
        case YAML_FLOW_SEQUENCE_END_TOKEN:
            break;
        case YAML_FLOW_MAPPING_START_TOKEN:
            object = malloc(sizeof(struct vinbero_com_Object));
            VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_MAP);
            for(; token.type != YAML_FLOW_MAPPING_END_TOKEN; yaml_parser_scan(parser, &token)) {
                switch(token.type) {
                case YAML_KEY_TOKEN:
                    keyObject = vinbero_com_Object_fromYaml(parser);
                    break;
                case YAML_VALUE_TOKEN:
                    childObject = vinbero_com_Object_fromYaml(parser);
                    GENC_MTREE_NODE_KEY(childObject) = keyObject;
                    GENC_MTREE_NODE_KEY_LENGTH(childObject) = sizeof(struct vinbero_com_Object);
                    GENC_MTREE_NODE_SET(object, childObject, &oldObject);
                    if(oldObject != NULL)
                        vinbero_com_Object_destroy(oldObject);
                    break;
                }
            }
            return object;
        case YAML_FLOW_MAPPING_END_TOKEN:
            break;
        case YAML_BLOCK_ENTRY_TOKEN:
            object = vinbero_com_Object_fromYaml(parser);
            return object;
        case YAML_FLOW_ENTRY_TOKEN:
            object = vinbero_com_Object_fromYaml(parser);
            return object;
        case YAML_KEY_TOKEN:
            break;
        case YAML_VALUE_TOKEN:
            break;
        case YAML_ALIAS_TOKEN:
            break;
        case YAML_ANCHOR_TOKEN:
            break;
        case YAML_TAG_TOKEN:
            break;
        case YAML_SCALAR_TOKEN:
            object = malloc(sizeof(struct vinbero_com_Object));
            VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_CONSTRING);
            VINBERO_COM_OBJECT_CONSTRING(object) = (const char*)token.data.scalar.value;
            return object;
        }
    }
    return NULL;
}
