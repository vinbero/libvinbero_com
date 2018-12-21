#ifndef _VINBERO_COMMON_OBJECT_H
#define _VINBERO_COMMON_OBJECT_H

struct vinbero_common_Object {
    union data {
       bool boolean;
       int integer;
       double real;
       char* string;
       GENC_TREE_NODE(struct vinbero_common_Object, struct vinbero_common_Object*);
       // GENC_HTREE_NODE(struct vinbero_common_Object, struct vinbero_common_Object*);
    };
    enum vinbero_common_Object_Type type;
    json_t* json;
};

#define VINBERO_COMMON_OBJECT_IS_BOOLEAN(object) \
object->type == VINBERO_COMMON_OBJECT_TYPE_BOOLEAN

#define VINBERO_COMMON_OBJECT_IS_INTEGER(object) \
object->type == VINBERO_COMMON_OBJECT_TYPE_INTEGER

#define VINBERO_COMMON_OBJECT_IS_REAL(object) \
object->type == VINBERO_COMMON_OBJECT_TYPE_REAL

#define VINBERO_COMMON_OBJECT_IS_STRING(object) \
object->type == VINBERO_COMMON_OBJECT_TYPE_STRING

#define VINBERO_COMMON_OBJECT_IS_ARRAY(object) \
object->type == VINBERO_COMMON_OBJECT_TYPE_ARRAY

#define VINBERO_COMMON_OBJECT_IS_MAP(object) \
object->type == VINBERO_COMMON_OBJECT_TYPE_MAP

#define VINBERO_COMMON_OBJECT_EVAL_BOOLEAN(object) \
object->data.boolean

#define VINBERO_COMMON_OBJECT_EVAL_INTEGER(object) \
object->data.integer

#define VINBERO_COMMON_OBJECT_EVAL_REAL(object) \
object->data.real

#define VINBERO_COMMON_OBJECT_EVAL_STRING(object, value, ret) \
object->data.string

#define VINBERO_COMMON_OBJECT_ARRAY_GET(object, index) \
GENC_TREE_NODE_GET_CHILD(object, index)

#define VINBERO_COMMON_OBJECT_ARRAY_GET_SIZE(object) \
GENC_TREE_NODE_GET_CHILD_COUNT(object)

#define VINBERO_COMMON_OBJECT_MAP_GET(object, key) \
GENC_HTREE_NODE_GET_CHILD(object, key)

#define VINBERO_COMMON_OBJECT_ARRAY_FOR_EACH_BEGIN(object, index, value) do { \
    GENC_TREE_NODE_FOR_EACH_CHILD(object, index) {

#define VINBERO_COMMON_OBJECT_ARRAY_FOR_EACH_END \
    } \
} while(0)

#define VINBERO_COMMON_OBJECT_MAP_FOR_EACH_BEGIN(object, key, value) do { \
    GENC_HTREE_NODE_FOR_EACH_CHILD(object, key) { \

#define VINBERO_COMMON_OBJECT_MAP_FOR_EACH_END \
    } \
} while(0)

#endif
