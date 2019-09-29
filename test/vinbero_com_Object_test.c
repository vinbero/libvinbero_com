#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <string.h>
#include "../src/vinbero_com_Config.h"
#include "../src/vinbero_com_Object.h"
#include "../src/vinbero_com_Status.h"

static void vinbero_com_Object_test(void** state) {
    struct vinbero_com_Object object;
    VINBERO_COM_OBJECT_INIT(&object, VINBERO_COM_OBJECT_TYPE_MAP);
    struct vinbero_com_Object object2;
    struct vinbero_com_Object* old;

    VINBERO_COM_OBJECT_INIT(&object2, VINBERO_COM_OBJECT_TYPE_INTEGER);
    VINBERO_COM_OBJECT_INTEGER(&object2) = 123;
    GENC_MTREE_NODE_KEY(&object2) = "HELLO";
    GENC_MTREE_NODE_KEY_LENGTH(&object2) = sizeof("HELLO") - 1;
    GENC_MTREE_NODE_SET(&object, &object2, &old);
    GENC_MTREE_NODE_GET(&object, "HELLO", sizeof("HELLO") - 1, &old);
    printf("%d\n", VINBERO_COM_OBJECT_INTEGER(old));

    struct vinbero_com_Config config;
    vinbero_com_Config_init(&config);
    assert_int_equal(vinbero_com_Config_fromFile(&config, "with_children.json"), VINBERO_COM_STATUS_SUCCESS);
    old = vinbero_com_Object_fromJson(config.config_object.json);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(vinbero_com_Object_test)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
