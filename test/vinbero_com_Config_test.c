#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include <libgenc/genc_Tree.h>
#include "../src/vinbero_com_Status.h"
#include "../src/vinbero_com_Config.h"
#include "../src/vinbero_com_Object.h"

static void test_Config_fromFile(void** state) {
    struct vinbero_com_Config config;

    vinbero_com_Config_init(&config);
    assert_int_equal(vinbero_com_Config_fromFile(&config, "wrong_syntax.json"), VINBERO_COM_ERROR_INVALID_CONFIG);
    vinbero_com_Config_destroy(&config);

    vinbero_com_Config_init(&config);
    assert_int_equal(vinbero_com_Config_fromFile(&config, "non_existing.json"), VINBERO_COM_ERROR_INVALID_CONFIG);
    vinbero_com_Config_destroy(&config);

    vinbero_com_Config_init(&config);
    assert_int_equal(vinbero_com_Config_fromFile(&config, "no_children.json"), VINBERO_COM_STATUS_SUCCESS);
    vinbero_com_Config_destroy(&config);

}

static void test_Config_check(void** state) {
    struct vinbero_com_Config config;

    vinbero_com_Config_init(&config);
    assert_int_equal(vinbero_com_Config_fromFile(&config, "no_next.json"), VINBERO_COM_STATUS_SUCCESS);
    assert_int_equal(vinbero_com_Config_check(&config, "core"), VINBERO_COM_ERROR_INVALID_CONFIG);

    vinbero_com_Config_destroy(&config);

    vinbero_com_Config_init(&config);
    assert_int_equal(vinbero_com_Config_fromFile(&config, "no_config.json"), VINBERO_COM_STATUS_SUCCESS);
    assert_int_equal(vinbero_com_Config_check(&config, "core"), VINBERO_COM_ERROR_INVALID_CONFIG);
    vinbero_com_Config_destroy(&config);

    vinbero_com_Config_init(&config);
    assert_int_equal(vinbero_com_Config_fromFile(&config, "no_children.json"), VINBERO_COM_STATUS_SUCCESS);
    assert_int_equal(vinbero_com_Config_check(&config, "core"), VINBERO_COM_STATUS_SUCCESS);
    vinbero_com_Config_destroy(&config);
}

static void test_Config_getChildModuleCount(void** state) {
    struct vinbero_com_Config config;

    vinbero_com_Config_init(&config);
    assert_int_equal(vinbero_com_Config_fromFile(&config, "with_children.json"), VINBERO_COM_STATUS_SUCCESS);
    assert_int_equal(vinbero_com_Config_check(&config, "core"), VINBERO_COM_STATUS_SUCCESS);
    assert_int_equal(vinbero_com_Config_getChildModuleCount(&config, "core"), 3);
    vinbero_com_Config_destroy(&config);
}

static void test_Config_getChildModuleIds(void** state) {
    struct vinbero_com_Config config;
    struct vinbero_com_Object* ids;
    struct vinbero_com_Object* id;

    vinbero_com_Config_init(&config);
    assert_int_equal(vinbero_com_Config_fromFile(&config, "with_children.json"), VINBERO_COM_STATUS_SUCCESS);
    assert_int_equal(vinbero_com_Config_check(&config, "core"), VINBERO_COM_STATUS_SUCCESS);
    assert_int_equal(vinbero_com_Config_getChildModuleIds(&config, "core", &ids), VINBERO_COM_STATUS_SUCCESS);
    assert_int_equal(GENC_TREE_NODE_SIZE(ids), 3);

    id = GENC_TREE_NODE_RAW_GET(ids, 0);
    assert_string_equal(VINBERO_COM_OBJECT_CONSTRING(id), "vinbero_dummy1");

    id = GENC_TREE_NODE_RAW_GET(ids, 1);
    assert_string_equal(VINBERO_COM_OBJECT_CONSTRING(id), "vinbero_dummy2");
    
    id = GENC_TREE_NODE_RAW_GET(ids, 2);
    assert_string_equal(VINBERO_COM_OBJECT_CONSTRING(id), "vinbero_dummy3");
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
