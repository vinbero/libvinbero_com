#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <string.h>
#include "../src/vinbero_common_Config.h"
#include "../src/vinbero_common_Object.h"
#include "../src/vinbero_common_Status.h"

static void vinbero_common_Object_test(void** state) {
    struct vinbero_common_Object object;
    VINBERO_COMMON_OBJECT_INIT(&object, VINBERO_COMMON_OBJECT_TYPE_MAP);
    struct vinbero_common_Object object2;
    struct vinbero_common_Object* old;

    VINBERO_COMMON_OBJECT_INIT(&object2, VINBERO_COMMON_OBJECT_TYPE_INTEGER);
    VINBERO_COMMON_OBJECT_INTEGER(&object2) = 123;
    GENC_MTREE_NODE_KEY(&object2) = "HELLO";
    GENC_MTREE_NODE_KEY_LENGTH(&object2) = sizeof("HELLO") - 1;
    GENC_MTREE_NODE_SET_CHILD(&object, &object2, &old);
    GENC_MTREE_NODE_GET_CHILD(&object, "HELLO", sizeof("HELLO") - 1, &old);
    printf("%d\n", VINBERO_COMMON_OBJECT_INTEGER(old));

    struct vinbero_common_Config config;
    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "with_children.json"), VINBERO_COMMON_STATUS_SUCCESS);
    old = vinbero_common_Object_fromJson(config.json);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(vinbero_common_Object_test)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
