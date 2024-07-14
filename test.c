#include "tqueue.h"

#include <assert.h>

#include <stdlib.h>
#include <stdio.h>

#include <errno.h>

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

int main()
{
    test_tryget_terminated_queue();
    test_tryget_empty_queue();
}
