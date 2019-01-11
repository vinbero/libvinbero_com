#ifndef _VINBERO_COMMON_OBJECT_H
#define _VINBERO_COMMON_OBJECT_H

#include <string.h>
#include <libgenc/genc_Tree.h>
#include <libgenc/genc_Mtree.h>

enum vinbero_common_Object_Type {
    VINBERO_COMMON_OBJECT_TYPE_BOOLEAN,
    VINBERO_COMMON_OBJECT_TYPE_INTEGER,
    VINBERO_COMMON_OBJECT_TYPE_REAL,
    VINBERO_COMMON_OBJECT_TYPE_STRING,
    VINBERO_COMMON_OBJECT_TYPE_ARRAY,
    VINBERO_COMMON_OBJECT_TYPE_MAP
};

struct vinbero_common_Object {
    enum vinbero_common_Object_Type type;
    union {
       bool boolean;
       int integer;
       double real;
       char* string;
       GENC_TREE_NODE(struct vinbero_common_Object, struct vinbero_common_Object*);
       GENC_MTREE_NODE(struct vinbero_common_Object, struct vinbero_common_Object*);
    };
};

#define VINBERO_COMMON_OBJECT_INIT(object, _type) { \
    memset(object, 0, sizeof(struct vinbero_common_Object)); \
    (object)->type = _type; \
    if(VINBERO_COMMON_OBJECT_IS_ARRAY(object)) { \
        GENC_TREE_NODE_INIT(object); \
    } else if(VINBERO_COMMON_OBJECT_IS_MAP(object)) { \
        GENC_MTREE_NODE_INIT(object); \
    } \
}

#define VINBERO_COMMON_OBJECT_IS_BOOLEAN(object) \
(object)->type == VINBERO_COMMON_OBJECT_TYPE_BOOLEAN

#define VINBERO_COMMON_OBJECT_IS_INTEGER(object) \
(object)->type == VINBERO_COMMON_OBJECT_TYPE_INTEGER

#define VINBERO_COMMON_OBJECT_IS_REAL(object) \
(object)->type == VINBERO_COMMON_OBJECT_TYPE_REAL

#define VINBERO_COMMON_OBJECT_IS_STRING(object) \
(object)->type == VINBERO_COMMON_OBJECT_TYPE_STRING

#define VINBERO_COMMON_OBJECT_IS_ARRAY(object) \
(object)->type == VINBERO_COMMON_OBJECT_TYPE_ARRAY

#define VINBERO_COMMON_OBJECT_IS_MAP(object) \
(object)->type == VINBERO_COMMON_OBJECT_TYPE_MAP

#define VINBERO_COMMON_OBJECT_EVAL_BOOLEAN(object) \
(object)->boolean

#define VINBERO_COMMON_OBJECT_EVAL_INTEGER(object) \
(object)->integer

#define VINBERO_COMMON_OBJECT_EVAL_REAL(object) \
(object)->real

#define VINBERO_COMMON_OBJECT_EVAL_STRING(object, value, ret) \
(object)->string

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

#define VINBERO_COMMON_OBJECT_FROM_JSON(json, object)

#endif
