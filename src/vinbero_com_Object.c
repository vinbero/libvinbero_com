#include <jansson.h>
#include <libgenc/genc_Tree.h>
#include <libgenc/genc_Mtree.h>
#include "vinbero_com_Object.h"

void vinbero_com_Object_destroy(struct vinbero_com_Object* object) {
    struct vinbero_com_Object* childObject;
    switch(VINBERO_COM_OBJECT_TYPE(object)) {
    case VINBERO_COM_OBJECT_TYPE_ARRAY:
        GENC_TREE_NODE_FOREACH(object, index) {
            vinbero_com_Object_destroy(GENC_TREE_NODE_RAW_GET(object, index));
        }
        break;
    case VINBERO_COM_OBJECT_TYPE_MAP:
        GENC_MTREE_NODE_FOREACH_BEGIN(object, &childObject)
            vinbero_com_Object_destroy(childObject);
        GENC_MTREE_NODE_FOREACH_END;
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
