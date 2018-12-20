#ifndef _VINBERO_COMMON_OBJECT_H
#define _VINBERO_COMMON_OBJECT_H

struct vinbero_common_Object {
    json_t* json;
};

#define VINBERO_COMMON_OBJECT_IS_BOOL(object) \
json_is_boolean((object)->json)

#define VINBERO_COMMON_OBJECT_IS_INT(object) \
json_is_integer((object)->json)

#define VINBERO_COMMON_OBJECT_IS_DOUBLE(object) \
json_is_real((object)->json)

#define VINBERO_COMMON_OBJECT_IS_STRING(object) \
json_is_string((object)->json)

#define VINBERO_COMMON_OBJECT_IS_ARRAY(object) \
json_is_array((object)->json)

#define VINBERO_COMMON_OBJECT_IS_MAP(object) \
json_is_object((object)->json)

#define VINBERO_COMMON_OBJECT_EVAL_BOOL(object) \
json_boolean_value((object)->json)

#define VINBERO_COMMON_OBJECT_EVAL_INT(object, value, ret) \
json_integer_value((object)->json)

#define VINBERO_COMMON_OBJECT_EVAL_DOUBLE(object, value, ret) \
json_real_value((object)->json)

#define VINBERO_COMMON_OBJECT_EVAL_STRING(object, value, ret) \
json_string_value((object)->json)

#define VINBERO_COMMON_OBJECT_ARRAY_GET(object, index, ret) \
json_array_get((object)->json, index)

#define VINBERO_COMMON_OBJECT_ARRAY_GET_SIZE(object) \
json_array_size((object)->json)

#define VINBERO_COMMON_OBJECT_MAP_GET(object, key, ret) \
json_object_get((object)->json, key)

#define VINBERO_COMMON_OBJECT_ARRAY_FOR_EACH_BEGIN(object, index, value) do { \
    json_array_foreach() {

#define VINBERO_COMMON_OBJECT_ARRAY_FOR_EACH_END(object, index, value) \
    } \
} while(0)

#define VINBERO_COMMON_OBJECT_MAP_FOR_EACH_BEGIN(object, key, value) do { \
    json_object_foreach() { \
#define VINBERO_COMMON_OBJECT_MAP_FOR_EACH_END \
    } \
} while(0)

#endif
