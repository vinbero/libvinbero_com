#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include <libgenc/genc_Tree.h>
#include "../src/vinbero_common_Status.h"
#include "../src/vinbero_common_Config.h"
#include "../src/vinbero_common_Object.h"

static void test_Config_fromFile(void** state) {
    struct vinbero_common_Config config;

    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "wrong_syntax.json"), VINBERO_COMMON_ERROR_INVALID_CONFIG);
    vinbero_common_Config_destroy(&config);

    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "non_existing.json"), VINBERO_COMMON_ERROR_INVALID_CONFIG);
    vinbero_common_Config_destroy(&config);

    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "no_children.json"), VINBERO_COMMON_STATUS_SUCCESS);
    vinbero_common_Config_destroy(&config);

}

static void test_Config_check(void** state) {
    struct vinbero_common_Config config;

    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "no_next.json"), VINBERO_COMMON_STATUS_SUCCESS);
    assert_int_equal(vinbero_common_Config_check(&config, "core"), false);

    vinbero_common_Config_destroy(&config);

    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "no_config.json"), VINBERO_COMMON_STATUS_SUCCESS);
    assert_int_equal(vinbero_common_Config_check(&config, "core"), false);
    vinbero_common_Config_destroy(&config);

    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "no_children.json"), VINBERO_COMMON_STATUS_SUCCESS);
    assert_int_equal(vinbero_common_Config_check(&config, "core"), true);
    vinbero_common_Config_destroy(&config);
}

static void test_Config_getChildModuleCount(void** state) {
    struct vinbero_common_Config config;

    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "with_children.json"), VINBERO_COMMON_STATUS_SUCCESS);
    assert_int_equal(vinbero_common_Config_check(&config, "core"), true);
    assert_int_equal(vinbero_common_Config_getChildModuleCount(&config, "core"), 3);
    vinbero_common_Config_destroy(&config);
}

static void test_Config_getChildModuleIds(void** state) {
    struct vinbero_common_Config config;
    struct vinbero_common_Object* ids;
    struct vinbero_common_Object* id;

    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "with_children.json"), VINBERO_COMMON_STATUS_SUCCESS);
    assert_int_equal(vinbero_common_Config_check(&config, "core"), true);
    ids = vinbero_common_Config_getChildModuleIds(&config, "core");
    assert_int_equal(GENC_TREE_NODE_CHILD_COUNT(ids), 3);

    id = GENC_TREE_NODE_GET_CHILD(ids, 0);
    assert_string_equal(VINBERO_COMMON_OBJECT_CONSTRING(id), "vinbero_dummy1");

    id = GENC_TREE_NODE_GET_CHILD(ids, 1);
    assert_string_equal(VINBERO_COMMON_OBJECT_CONSTRING(id), "vinbero_dummy2");
    
    id = GENC_TREE_NODE_GET_CHILD(ids, 2);
    assert_string_equal(VINBERO_COMMON_OBJECT_CONSTRING(id), "vinbero_dummy3");
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_Config_fromFile),
        cmocka_unit_test(test_Config_check),
        cmocka_unit_test(test_Config_getChildModuleCount),
        cmocka_unit_test(test_Config_getChildModuleIds)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
