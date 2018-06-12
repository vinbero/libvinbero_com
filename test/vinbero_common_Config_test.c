#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../src/vinbero_common_Status.h"
#include "../src/vinbero_common_Config.h"

void test_vinbero_common_Config(void** state) {
    struct vinbero_common_Config config;

    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "test.json"), VINBERO_COMMON_STATUS_SUCCESS);
    assert_int_equal(vinbero_common_Config_check(&config, "core"), VINBERO_COMMON_STATUS_SUCCESS);
    vinbero_common_Config_destroy(&config);

    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "test2.json"), VINBERO_COMMON_STATUS_SUCCESS);
    assert_int_equal(vinbero_common_Config_check(&config, "core"), VINBERO_COMMON_ERROR_INVALID_CONFIG);
    vinbero_common_Config_destroy(&config);

    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "test3.json"), VINBERO_COMMON_STATUS_SUCCESS);
    assert_int_equal(vinbero_common_Config_check(&config, "core"), VINBERO_COMMON_ERROR_INVALID_CONFIG);
    vinbero_common_Config_destroy(&config);

    vinbero_common_Config_init(&config);
    assert_int_equal(vinbero_common_Config_fromFile(&config, "test4.json"), VINBERO_COMMON_STATUS_SUCCESS);
    assert_int_equal(vinbero_common_Config_check(&config, "core"), VINBERO_COMMON_STATUS_SUCCESS);
    assert_int_equal(vinbero_common_Config_getChildModuleCount(&config, "core"), 1);
    vinbero_common_Config_destroy(&config);

}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_vinbero_common_Config)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
