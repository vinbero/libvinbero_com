#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include "../src/vinbero_common_Object.h"

static void vinbero_common_Object_test(void** state) {
    struct vinbero_common_Object object;
    VINBERO_COMMON_OBJECT_INIT(&object, VINBERO_COMMON_OBJECT_TYPE_MAP);
}

int main() {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(vinbero_common_Object_test)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
