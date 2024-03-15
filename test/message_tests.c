#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <check.h>

START_TEST (sanity_test)
{
    ck_assert(true);
}
END_TEST

Suite *sanity_testsuite(void) {
    Suite *suite;
    TCase *tc_core;
    suite = suite_create("sanity testsuite");
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, sanity_test);
    suite_add_tcase(suite, tc_core);
    return suite;
};

int main(void) {
    printf("Running tests");

    int number_failed;
    Suite *test_suite;
    SRunner *suite_runner;
    test_suite = sanity_testsuite();

    suite_runner = srunner_create(test_suite);
    srunner_run_all(suite_runner, CK_NORMAL);

    number_failed = srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);

    printf("Running tests finished. Number of failed tests: %d \n", number_failed);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}