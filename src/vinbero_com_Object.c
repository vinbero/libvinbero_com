#include <jansson.h>
#include <libgenc/genc_Tree.h>
#include <libgenc/genc_Mtree.h>
#include "vinbero_com_Object.h"

void vinbero_com_Object_yaml_get_next_token(yaml_parser_t* parser, yaml_token_t* token);
char* vinbero_com_Object_yaml_get_next_token_type(yaml_parser_t* parser, yaml_token_t* token, yaml_token_type_t type);
struct vinbero_com_Object* vinbero_com_Object_Constring_fromStr(const char* str);

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
struct vinbero_com_Object* vinbero_com_Object_fromYaml(yaml_parser_t* parser, const char* key)
{
    struct vinbero_com_Object* object = malloc(sizeof(struct vinbero_com_Object));
    struct vinbero_com_Object* childObject;
    struct vinbero_com_Object* oldObject;
    yaml_token_t token;

    // token.type = YAML_VALUE_TOKEN
    vinbero_com_Object_yaml_get_next_token(parser, &token);

    /* 3 Possibility of token after value token.
     * [value token] - [scalar : string]
     *                  ex) port: 80
     *               - [mapping token] : continues with key:value ... so on. (recursive)
     *                  ex) ip:
     *                          address: 1.2.3.4
     *                          ttl: 128
     *               - [seqence of values] : iterate until seqence ends.
     *                  ex) lib:
     *                          - libc
     *                          - libyaml
     *                          - libpcap
    */
    vinbero_com_Object_yaml_get_next_token(parser, &token);

    switch (token.type)
    {
    /* just a value string */
    case YAML_SCALAR_TOKEN: {
        // child object is a const string object.
        childObject = vinbero_com_Object_Constring_fromStr((const char*)token.data.scalar.value);

        VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_MAP);
        GENC_MTREE_NODE_KEY(childObject) = key;
        GENC_MTREE_NODE_KEY_LENGTH(childObject) = strlen(key);
        GENC_MTREE_NODE_SET(object, childObject, &oldObject);
        break;
    }
    /* value is key-value again */
    case YAML_BLOCK_MAPPING_START_TOKEN: {
        VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_MAP);
        do {
            // key scalar str
            vinbero_com_Object_yaml_get_next_token(parser, &token);
            childObject = vinbero_com_Object_fromYaml(parser, (const char*)token.data.scalar.value);

            GENC_MTREE_NODE_KEY(childObject) = key;
            GENC_MTREE_NODE_KEY_LENGTH(childObject) = strlen(key);
            GENC_MTREE_NODE_SET(object, childObject, &oldObject);
            
            // must be key or block_end
            vinbero_com_Object_yaml_get_next_token(parser, &token);

            // if (token.type != YAML_KEY_TOKEN && token.type != YAML_BLOCK_END_TOKEN) error!!
        } while (token.type != YAML_BLOCK_END_TOKEN);
        break;
    }
    /* value is a list */
    case YAML_BLOCK_SEQUENCE_START_TOKEN: {
        VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_ARRAY);
        do {
            vinbero_com_Object_yaml_get_next_token(parser, &token);
            if (token.type == YAML_SCALAR_TOKEN) {
                childObject = vinbero_com_Object_Constring_fromStr((const char*)token.data.scalar.value);

                GENC_TREE_NODE_ADD(object, childObject);
            }
        } while(token.type != YAML_BLOCK_END_TOKEN);
        break;
    }
    default:
        free(object);
        object = NULL;
        break;
    }

    return object;
}

/*
 * build VINBERO_COM_OBJECT_TYPE_CONSTRING.
 * you must free object after use.
 * 
 * @param[IN]   str    string that build VINBERO_COM_OBJECT_TYPE_CONSTRING
 * 
*/
struct vinbero_com_Object* vinbero_com_Object_Constring_fromStr(const char* str)
{
    struct vinbero_com_Object* object = malloc(sizeof(struct vinbero_com_Object));
    VINBERO_COM_OBJECT_INIT(object, VINBERO_COM_OBJECT_TYPE_CONSTRING);
    VINBERO_COM_OBJECT_CONSTRING(object) = str;

    return object;
}

/*
 * Get next token which fits the type caller want.
 * 
 * @param[IN]    parser    yaml parser object
 * @param[OUT]   token     return next token here
 * @param[IN]    type      the token type caller want
*/
char* vinbero_com_Object_yaml_get_next_token_type(yaml_parser_t* parser, yaml_token_t* token, yaml_token_type_t type)
{
    do {
        yaml_parser_scan(parser, token);
    } while( token->type != type );
    // token must be scalar(key string)
    yaml_parser_scan(parser, token);
    return (char*)token->data.scalar.value;
}

/*
 * Get next token
 * 
 * @param[IN]    parser    yaml parser object
 * @param[OUT]   token     return next token here
*/
void vinbero_com_Object_yaml_get_next_token(yaml_parser_t* parser, yaml_token_t* token)
{
    yaml_parser_scan(parser, token);
}

/*
 * destory yaml_parser
 * 
 * @param[IN]   parser    yaml parser object
 * 
*/
void vinbero_com_Object_yaml_destory_yaml_parser(yaml_parser_t* parser)
{
    yaml_parser_delete(parser);
}
