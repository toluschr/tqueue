#ifndef TQUEUE_H_
#define TQUEUE_H_

#include <semaphore.h>

struct tqueue_node {
    struct tqueue_node *next;
    char data[];
};

struct tqueue {
    struct tqueue_node *head;
    struct tqueue_node *tail;

    sem_t sem_prod_cons;
    sem_t sem_data;
};

/**
 * \brief Initialize tqueue instance
 *
 * Initializes queue internal data structures and creates an empty queue.
 * Other calls must only be made after tqueue_init.
 *
 * \param q the tqueue instance to initialize
 */
int tqueue_init(struct tqueue *q);

/**
 * \brief Gets the current queue size
 *
 * Calls sem_getvalue on the underlying producer consumer lock.
 * I
 */
// int tqueue_size(struct tqueue *q);

/**
 * \brief appends a node to the end queue
 *
 * Appends a given node to the end of the queue. The underlying memory must be
 * managed by the user. If the given node is NULL, the queue will be
 * NULL-terminated. In case of any error, the queue will not be modified.
 *
 * \param n node to append to queue
 *
 * \exception EOVERFLOW the queue is full
 * \exception EINTR interrupted by a signal
 */
int tqueue_put_node(struct tqueue *q, struct tqueue_node *n);

/**
 * \brief removes the first node from the queue
 *
 * \exception EINTR interrupted by a signal
 */
int tqueue_get_node(struct tqueue *q, struct tqueue_node **n);

/**
 * \brief tries to removes the first node from the queue
 *
 * \exception EAGAIN the queue doesn't contain an element
 * \exception EINTR interrupted by a signal
 */
int tqueue_tryget_node(struct tqueue *q, struct tqueue_node **n);

/**
 * \brief tries to destroy the queue
 *
 * Tries to destroy the queue. Calling any queue function after tqueue_fini
 * is undefined.
 *
 * \exception EINVAL the queue is not a valid queue
 */
int tqueue_fini(struct tqueue *q);

#define tqueue_sizeof(t) (sizeof(struct tqueue_node) + sizeof(t))

#endif // TQUEUE_H_
