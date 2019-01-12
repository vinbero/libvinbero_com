#include <jansson.h>
#include <libgenc/genc_Tree.h>
#include <libgenc/genc_Mtree.h>
#include "vinbero_common_Object.h"

void vinbero_common_Object_destroy(struct vinbero_common_Object* object) {
    struct vinbero_common_Object* childObject;
    switch(VINBERO_COMMON_OBJECT_TYPE(object)) {
    case VINBERO_COMMON_OBJECT_TYPE_ARRAY:
        GENC_TREE_NODE_FOR_EACH_CHILD(object, index) {
            vinbero_common_Object_destroy(GENC_TREE_NODE_GET_CHILD(object, index));
        }
        break;
    case VINBERO_COMMON_OBJECT_TYPE_MAP:
        GENC_MTREE_NODE_FOR_EACH_CHILD_BEGIN(object, &childObject)
            vinbero_common_Object_destroy(childObject);
        GENC_MTREE_NODE_FOR_EACH_CHILD_END;
        break;
    }
    free(object);
}

struct vinbero_common_Object* vinbero_common_Object_fromJson(json_t* json) {
    struct vinbero_common_Object* object = malloc(sizeof(struct vinbero_common_Object));
    struct vinbero_common_Object* childObject;
    struct vinbero_common_Object* oldObject;
    size_t index;
    const char* key;
    json_t* value;
    enum vinbero_common_Object_Type type;
    switch(json_typeof(json)) {
    case JSON_NULL:
        VINBERO_COMMON_OBJECT_INIT(object, VINBERO_COMMON_OBJECT_TYPE_NULL);
        break;
    case JSON_FALSE:
        VINBERO_COMMON_OBJECT_INIT(object, VINBERO_COMMON_OBJECT_TYPE_BOOLEAN);
        VINBERO_COMMON_OBJECT_BOOLEAN(object) = false;
        break;
    case JSON_TRUE:
        VINBERO_COMMON_OBJECT_INIT(object, VINBERO_COMMON_OBJECT_TYPE_BOOLEAN);
        VINBERO_COMMON_OBJECT_BOOLEAN(object) = true;
        break;
    case JSON_INTEGER:
        VINBERO_COMMON_OBJECT_INIT(object, VINBERO_COMMON_OBJECT_TYPE_INTEGER);
        VINBERO_COMMON_OBJECT_INTEGER(object) = json_integer_value(json);
        break;
    case JSON_REAL:
        VINBERO_COMMON_OBJECT_INIT(object, VINBERO_COMMON_OBJECT_TYPE_REAL);
        VINBERO_COMMON_OBJECT_REAL(object) = json_real_value(json);
        break;
    case JSON_STRING:
        VINBERO_COMMON_OBJECT_INIT(object, VINBERO_COMMON_OBJECT_TYPE_CONSTRING);
        VINBERO_COMMON_OBJECT_CONSTRING(object) = json_string_value(json);
        break;
    case JSON_ARRAY:
        VINBERO_COMMON_OBJECT_INIT(object, VINBERO_COMMON_OBJECT_TYPE_ARRAY);
        json_array_foreach(json, index, value) {
            childObject = vinbero_common_Object_fromJson(value);
            if(childObject == NULL) {
                vinbero_common_Object_destroy(object);
                object = NULL;
            }
            GENC_TREE_NODE_ADD_CHILD(object, childObject);
        }
        break;
    case JSON_OBJECT:
        VINBERO_COMMON_OBJECT_INIT(object, VINBERO_COMMON_OBJECT_TYPE_MAP);
        VINBERO_COMMON_OBJECT_INIT(object, VINBERO_COMMON_OBJECT_TYPE_MAP);
        json_object_foreach(json, key, value) {
            childObject = vinbero_common_Object_fromJson(value);
            if(childObject == NULL) {
                vinbero_common_Object_destroy(object);
                object = NULL;
            }
            GENC_MTREE_NODE_KEY(childObject) = key;
            GENC_MTREE_NODE_KEY_LENGTH(childObject) = strlen(key);
            GENC_MTREE_NODE_SET_CHILD(object, childObject, &oldObject);
        }
        break;
    default:
        free(object);
        object = NULL;
    }
    return object;
}


