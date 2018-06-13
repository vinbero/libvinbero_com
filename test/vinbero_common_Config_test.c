#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../src/vinbero_common_Status.h"
#include "../src/vinbero_common_Config.h"

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
    assert_int_equal(vinbero_common_Config_check(&config, "core"), VINBERO_COMMON_ERROR_INVALID_CONFIG);
    vinbero_common_Config_destroy(&config);

    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "no_config.json"), VINBERO_COMMON_STATUS_SUCCESS);
    assert_int_equal(vinbero_common_Config_check(&config, "core"), VINBERO_COMMON_ERROR_INVALID_CONFIG);
    vinbero_common_Config_destroy(&config);

    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "no_children.json"), VINBERO_COMMON_STATUS_SUCCESS);
    assert_int_equal(vinbero_common_Config_check(&config, "core"), VINBERO_COMMON_STATUS_SUCCESS);
    vinbero_common_Config_destroy(&config);

}

static void test_Config_getChildModuleCount(void** state) {
    struct vinbero_common_Config config;

    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "with_children.json"), VINBERO_COMMON_STATUS_SUCCESS);
    assert_int_equal(vinbero_common_Config_check(&config, "core"), VINBERO_COMMON_STATUS_SUCCESS);
    assert_int_equal(vinbero_common_Config_getChildModuleCount(&config, "core"), 3);
    vinbero_common_Config_destroy(&config);

}

static void test_Config_getChildModuleIds(void** state) {
    struct vinbero_common_Config config;
    struct vinbero_common_Module_Ids ids;

    vinbero_common_Module_Ids_init(&ids);
    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "with_children.json"), VINBERO_COMMON_STATUS_SUCCESS);
    assert_int_equal(vinbero_common_Config_check(&config, "core"), VINBERO_COMMON_STATUS_SUCCESS);
    vinbero_common_Config_getChildModuleIds(&config, "core", &ids);
    assert_int_equal(GENC_ARRAY_LIST_SIZE(&ids), 3);
    assert_string_equal(GENC_ARRAY_LIST_GET(&ids, 0), "vinbero_dummy1");
    assert_string_equal(GENC_ARRAY_LIST_GET(&ids, 1), "vinbero_dummy2");
    assert_string_equal(GENC_ARRAY_LIST_GET(&ids, 2), "vinbero_dummy3");
    vinbero_common_Module_Ids_destroy(&ids);
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
