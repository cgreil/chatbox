#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <check.h>

#include "../client/client.h"
#include "../client/user.h"
#include "../client/message.h"

START_TEST (sanity_test) {
    ck_assert(true);
}

END_TEST

START_TEST (create_message_test) {
    message_t message;

    user_t test_user;
    test_user.username = "christoph";
    test_user.user_id = 42;

    char *content = "This is a test";
    size_t content_len = strlen(content);

    create_message(&message, PUBLIC_MESSAGE, &test_user, &content, content_len);
    char timestring[20];
    time_from_string(timestring, message.creation_timestamp);
    printf("Message is: "
           "Type: %u \n"
           "Creation time: %s \n"
           "Sendername: %s \n"
           "Content: %s",
           message.message_type,
           timestring,
           message.sender->username,
           message.message_content);

    ck_assert(true);
}
END_TEST

START_TEST (serialize_msg_test) {

}

END_TEST

START_TEST (deserialize_msg_test) {

}

END_TEST

START_TEST (serialization_deserialization_test) {

}

END_TEST

Suite *message_testsuite(void) {
    Suite *suite;
    suite = suite_create("Message tests");

    TCase *sanity_testcase;
    sanity_testcase = tcase_create("Sanity Check");
    tcase_add_test(sanity_testcase, sanity_test);

    TCase *create_msg_testcase;
    create_msg_testcase = tcase_create("Message Creation");
    tcase_add_test(create_msg_testcase, create_message_test);


    suite_add_tcase(suite, sanity_testcase);
    suite_add_tcase(suite, create_msg_testcase);
    return suite;
};

int main(void) {
    printf("Running tests");
    int number_failed;

    Suite *test_suite;
    SRunner *suite_runner;
    test_suite = message_testsuite();

    suite_runner = srunner_create(test_suite);
    srunner_run_all(suite_runner, CK_NORMAL);

    number_failed = srunner_ntests_failed(suite_runner);
    srunner_free(suite_runner);

    printf("Running tests finished. Number of failed tests: %d \n", number_failed);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
