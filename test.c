#include "tqueue.h"

#include <assert.h>

#include <stdlib.h>
#include <stdio.h>

#include <errno.h>

static void test_queue_insert_and_remove_with_end(void)
{
    struct tqueue q;
    assert(tqueue_init(&q) == 0);

    struct tqueue_node *i = malloc(tqueue_sizeof(int));
    assert(i != NULL);

    *(int *)i->data = 0;

    while (tqueue_put_node(&q, i) < 0) {
        assert(errno == EINTR);
    }

    while (tqueue_put_node(&q, NULL) < 0) {
        assert(errno == EINTR);
    }

    while (tqueue_get_node(&q, &i) < 0) {
        assert(errno == EINTR);
    }

    assert(i != NULL);
    free(i);

    while (tqueue_get_node(&q, &i) < 0) {
        assert(errno == EINTR);
    }

    assert(i == NULL);

    assert(tqueue_fini(&q) == 0);
}

static void test_queue_length_zero_with_end(void)
{
    struct tqueue q;
    assert(tqueue_init(&q) == 0);

    while (tqueue_put_node(&q, NULL) < 0) {
        assert(errno == EINTR);
    }

    assert(tqueue_length(&q) == 0);

    assert(tqueue_fini(&q) == 0);
}

static void test_queue_length_zero_without_end(void)
{
    struct tqueue q;
    assert(tqueue_init(&q) == 0);

    assert(tqueue_length(&q) == 0);

    assert(tqueue_fini(&q) == 0);
}

static void test_queue_length_with_end(void)
{
    struct tqueue q;
    struct tqueue_node *i;

    assert(tqueue_init(&q) == 0);

    i = malloc(tqueue_sizeof(int));
    assert(i != NULL);
    *(int *)i->data = 0;

    while (tqueue_put_node(&q, i) < 0) {
        assert(errno == EINTR);
    }

    i = malloc(tqueue_sizeof(int));
    assert(i != NULL);
    *(int *)i->data = 1;

    while (tqueue_put_node(&q, i) < 0) {
        assert(errno == EINTR);
    }

    while (tqueue_put_node(&q, NULL) < 0) {
        assert(errno == EINTR);
    }

    assert(tqueue_length(&q) == 2);

    while (tqueue_tryget_node(&q, &i) < 0) {
        assert(errno == EINTR);
    }

    assert(i != NULL);
    free(i);

    while (tqueue_tryget_node(&q, &i) < 0) {
        assert(errno == EINTR);
    }

    assert(i != NULL);
    free(i);

    while (tqueue_tryget_node(&q, &i) < 0) {
        assert(errno == EINTR);
    }

    assert(i == NULL);

    assert(tqueue_length(&q) == 0);

    assert(tqueue_fini(&q) == 0);
}

static void test_queue_length_without_end(void)
{
    struct tqueue q;
    assert(tqueue_init(&q) == 0);

    struct tqueue_node *i = malloc(tqueue_sizeof(int));
    assert(i != NULL);
    *(int *)i->data = 1;

    while (tqueue_put_node(&q, i) < 0) {
        assert(errno == EINTR);
    }

    while (tqueue_put_node(&q, i) < 0) {
        assert(errno == EINTR);
    }

    assert(tqueue_length(&q) == 2);

    while (tqueue_tryget_node(&q, &i) < 0) {
        assert(errno == EINTR);
    }

    while (tqueue_tryget_node(&q, &i) < 0) {
        assert(errno == EINTR);
    }

    free(i);

    assert(tqueue_length(&q) == 0);

    assert(tqueue_fini(&q) == 0);
}

static void test_tryget_terminated_queue(void)
{
    struct tqueue q;
    assert(tqueue_init(&q) == 0);

    struct tqueue_node *i = malloc(tqueue_sizeof(int));
    assert(i != NULL);

    *(int *)i->data = 1;

    while (tqueue_put_node(&q, i) < 0) {
        assert(errno == EINTR);
    }

    while (tqueue_put_node(&q, NULL) < 0) {
        assert(errno == EINTR);
    }

    assert(tqueue_tryget_node(&q, &i) == 0);
    assert(i && (*(int *)i->data) == 1);
    free(i);

    assert(tqueue_tryget_node(&q, &i) == 0);
    assert(i == NULL);

    assert(tqueue_fini(&q) == 0);
}

static void test_tryget_empty_queue(void)
{
    struct tqueue q;
    assert(tqueue_init(&q) == 0);

    struct tqueue_node *i = malloc(tqueue_sizeof(int));
    assert(i != NULL);

    *(int *)i->data = 1;

    while (tqueue_put_node(&q, i) < 0) {
        assert(errno == EINTR);
    }

    assert(tqueue_tryget_node(&q, &i) == 0);
    assert(i && (*(int *)i->data) == 1);
    free(i);

    assert(tqueue_tryget_node(&q, &i) < 0 && errno == EAGAIN);

    assert(tqueue_fini(&q) == 0);
}

int main(void)
{
    test_queue_insert_and_remove_with_end();
    test_queue_length_zero_with_end();
    test_queue_length_zero_without_end();
    test_queue_length_with_end();
    test_queue_length_without_end();
    test_tryget_terminated_queue();
    test_tryget_empty_queue();
}
