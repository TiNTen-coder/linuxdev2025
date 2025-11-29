#include <check.h>
#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <assert.h>
#include <sys/resource.h>
#include "grow.h"

float *a = NULL;
long *ai = 0;
void init_setup(void){
    float *a = 0;
}

void init_teardown(void){
    if (a != NULL){
        buf_free(a);
    }
}

START_TEST(test_init_capacity){
    assert(buf_capacity(a) == 0);
} END_TEST

START_TEST(test_init_size){
    assert(buf_size(a) == 0);
} END_TEST

START_TEST(test_init_size1){
    buf_push(a, 1.3f);
    assert(buf_size(a) == 1);
} END_TEST

START_TEST(test_init_value){
    buf_push(a, 1.3f);
    assert(a[0] == (float)1.3f);
} END_TEST

START_TEST(test_init_clear){
    buf_push(a, 1.3f);
    buf_clear(a);
    assert(buf_size(a) == 0);
} END_TEST

START_TEST(test_init_clear_non_free){
    buf_push(a, 1.3f);
    buf_clear(a);
    assert(a != 0);
} END_TEST

START_TEST(test_init_free){
    buf_push(a, 1.3f);
    buf_clear(a);
    buf_free(a);
    assert(a == 0);
} END_TEST

START_TEST(test_init_null_empty){
    buf_free(a);
    buf_clear(a);
    assert(buf_size(a) == 0);
} END_TEST

START_TEST(test_init_clear_noop){
    buf_free(a);
    buf_clear(a);
    assert(a == 0);
} END_TEST

void push_setup(void){
    for (int i = 0; i < 10000; i++)
        buf_push(ai, i);
}

void push_teardown(void){
    if (ai != NULL){
        buf_free(ai);
    }
}

START_TEST(test_push_size){
    assert(buf_size(ai) == 10000);
} END_TEST

START_TEST(test_push_match){
    int match = 0;
    for (int i = 0; i < (int)(buf_size(ai)); i++)
        match += ai[i] == i;
    assert(match == 10000);
} END_TEST

void grow_setup(void){
    buf_grow(ai, 1000);
}

void grow_teardown(void){
    if (ai != NULL){
        buf_free(ai);
    }
}

START_TEST(grow_capacity){
    assert(buf_capacity(ai) == 1000);
} END_TEST

START_TEST(grow_size){
    assert(buf_size(ai) == 0);
} END_TEST

START_TEST(grow_trunc){
    buf_trunc(ai, 100);
    assert(buf_capacity(ai) == 100);
} END_TEST

void pop_setup(void){
    buf_push(a, 1.1);
    buf_push(a, 1.2);
    buf_push(a, 1.3);
    buf_push(a, 1.4);
}

void pop_teardown(void){
    if (a != NULL){
        buf_free(a);
    }
}

START_TEST(pop_size){
    assert(buf_size(a) == 4);
} END_TEST

START_TEST(pop_shrink){
    buf_trunc(a, 3);
    assert(buf_size(a) == 3);
} END_TEST

START_TEST(pop_pop){
    assert(buf_pop(a) == (float)1.4f);
} END_TEST

START_TEST(pop_pop_size){
    buf_pop(a);
    buf_trunc(a, 3);
    assert(buf_size(a) == 3);
} END_TEST

START_TEST(pop_pop2){
    buf_pop(a);
    buf_trunc(a, 3);
    assert(buf_pop(a) == (float)1.3f);
} END_TEST

START_TEST(pop_pop1){
    buf_pop(a);
    buf_trunc(a, 3);
    buf_pop(a);
    assert(buf_pop(a) == (float)1.2f);
} END_TEST

START_TEST(pop_pop0){
    buf_pop(a);
    buf_trunc(a, 3);
    buf_pop(a);
    buf_pop(a);
    assert(buf_pop(a) == (float)1.1f);
} END_TEST

START_TEST(pop_size0){
    buf_pop(a);
    buf_trunc(a, 3);
    buf_pop(a);
    buf_pop(a);
    buf_pop(a);
    assert(buf_size(a) == 0);
} END_TEST


START_TEST(overflow_nomem_malloc){
    const struct rlimit lim = {10000, 10000};
    setrlimit(RLIMIT_AS, &lim);
    buf_grow(a, 100000);
} END_TEST

START_TEST(overflow_nomem_realloc){
    const struct rlimit lim = {10000, 10000};
    buf_push(a, 1.0f);
    setrlimit(RLIMIT_AS, &lim);
    buf_grow(a, 100000);
} END_TEST

START_TEST(overflow_init){
    buf_trunc(a, PTRDIFF_MAX);
} END_TEST

START_TEST(overflow_grow){
    buf_trunc(a, 1);
    buf_trunc(a, PTRDIFF_MAX);
} END_TEST

int main(int argc, char *argv[]){
    Suite *suite = suite_create("tests");
    TCase *c_init = tcase_create("init");
    TCase *c_push = tcase_create("push");
    TCase *c_grow = tcase_create("grow");
    TCase *c_pop = tcase_create("pop");
    TCase *c_overflow = tcase_create("overflow");
    SRunner *runner = srunner_create(suite);
    int ret;

    suite_add_tcase(suite, c_init);
    tcase_add_checked_fixture(c_init, init_setup, init_teardown);
    tcase_add_test(c_init, test_init_capacity);
    tcase_add_test(c_init, test_init_size);
    tcase_add_test(c_init, test_init_size1);
    tcase_add_test(c_init, test_init_value);
    tcase_add_test(c_init, test_init_clear_non_free);
    tcase_add_test(c_init, test_init_free);
    tcase_add_test(c_init, test_init_null_empty);
    tcase_add_test(c_init, test_init_clear_noop);

    suite_add_tcase(suite, c_push);
    tcase_add_unchecked_fixture(c_push, push_setup, push_teardown);
    tcase_add_test(c_push, test_push_size);
    tcase_add_test(c_push, test_push_match);

    suite_add_tcase(suite, c_grow);
    tcase_add_unchecked_fixture(c_grow, grow_setup, grow_teardown);
    tcase_add_test(c_grow, grow_capacity);
    tcase_add_test(c_grow, grow_size);
    tcase_add_test(c_grow, grow_trunc);

    suite_add_tcase(suite, c_pop);
    tcase_add_unchecked_fixture(c_pop, pop_setup, pop_teardown);
    tcase_add_test(c_pop, pop_size);
    tcase_add_test(c_pop, pop_shrink);
    tcase_add_test(c_pop, pop_pop);
    tcase_add_test(c_pop, pop_pop_size);
    tcase_add_test(c_pop, pop_pop2);
    tcase_add_test(c_pop, pop_pop1);
    tcase_add_test(c_pop, pop_pop0);
    tcase_add_test(c_pop, pop_size0);

    suite_add_tcase(suite, c_overflow);
    tcase_add_checked_fixture(c_overflow, NULL, NULL);
    tcase_add_test_raise_signal(c_overflow, overflow_nomem_malloc, SIGABRT);
    tcase_add_test_raise_signal(c_overflow, overflow_nomem_realloc, SIGABRT);
    tcase_add_test_raise_signal(c_overflow, overflow_init, SIGABRT);
    tcase_add_test_raise_signal(c_overflow, overflow_grow, SIGABRT);

    srunner_run_all(runner, CK_ENV);
    ret = srunner_ntests_failed(runner);
    srunner_free(runner);
    return ret != 0;
} 
