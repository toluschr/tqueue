#include "tqueue.h"

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <errno.h>

static int remove_node(struct tqueue *q, struct tqueue_node **n)
{
    if (sem_wait(&q->sem_data) < 0) {
        assert(errno == EINTR);
        return -1;
    }

    if (n != NULL) {
        *n = q->head;
    }

    if (q->head && q->tail) {
        q->head = q->head->next;

        if (q->head == NULL) {
            q->tail = NULL;
        }
    }

    // EOVERFLOW impossible, sem_wait called by tqueue
    assert(sem_post(&q->sem_data) != -1);
    return 0;
}

int tqueue_init(struct tqueue *q)
{
    q->head = NULL;
    q->tail = NULL;

    if (sem_init(&q->sem_prod_cons, 0, 0) < 0) {
        assert(errno == EINVAL);
        return -1;
    }

    if (sem_init(&q->sem_data, 0, 1) < 0) {
        // EINVAL impossible, initialized above.
        assert(sem_destroy(&q->sem_prod_cons) != -1);
        assert(errno == EINVAL);
        return -1;
    }

    return 0;
}

int tqueue_put_node(struct tqueue *q, struct tqueue_node *n)
{
    if (n == NULL) {
        return sem_post(&q->sem_prod_cons);
    }

    n->next = NULL;

    if (sem_wait(&q->sem_data) < 0) {
        assert(errno == EINTR);
        return -1;
    }

    if (sem_post(&q->sem_prod_cons) == -1) {
        // EOVERFLOW impossible, sem_wait called by tqueue
        assert(sem_post(&q->sem_data) != -1);
        return -1;
    }

    if (!q->tail) {
        q->head = q->tail = n;
    } else {
        q->tail->next = n;
    }

    // EOVERFLOW impossible, sem_wait called by tqueue
    assert(sem_post(&q->sem_data) != -1);
    return 0;
}

int tqueue_get_node(struct tqueue *q, struct tqueue_node **n)
{
    if (sem_wait(&q->sem_prod_cons) < 0) {
        assert(errno == EINTR);
        return -1;
    }

    return remove_node(q, n);
}

int tqueue_tryget_node(struct tqueue *q, struct tqueue_node **n)
{
    if (sem_trywait(&q->sem_prod_cons) < 0) {
        // other errors impossible, specified by sem_trywait manpage
        assert(errno == EAGAIN || errno == EINTR);
        return -1;
    }

    return remove_node(q, n);
}

int tqueue_fini(struct tqueue *q)
{
    if (sem_destroy(&q->sem_prod_cons) < 0) {
        return -1;
    }

    // EINVAL impossible, ensured by queue functions
    assert(sem_destroy(&q->sem_data) != -1);
    return 0;
}
