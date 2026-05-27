// Unit test / mocking framework, cmocka:
#include <cmocka.h>

// Functionality we are testing:
#include <argv_lib.h>

static void null_test_success(void **data) {
    (void)data;
    
    print_argv(0, NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_test_success),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
