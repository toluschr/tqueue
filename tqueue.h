#ifndef TQUEUE_H_
#define TQUEUE_H_

#include <semaphore.h>

/**
 * @brief tqueue_node structure for tqueue elements
 */
struct tqueue_node {
    struct tqueue_node *next; /**< Pointer to the next element in the queue */
    char data[]; /**< Data of arbitrary size */
};

/**
 * @brief tqueue structure
 */
struct tqueue {
    struct tqueue_node *head; /**< Pointer to the head of the queue */
    struct tqueue_node *tail; /**< Pointer to the tail of the queue */

    sem_t sem_prod_cons; /**< Semaphore for producer-consumer synchronization */
    sem_t sem_data; /**< Semaphore for data access synchronization */
};

/**
 * @brief Macro to calculate a nodes allocation size
 *
 * @param t The type of data stored in the tqueue_node
 *
 * @return The full size of the tqueue_node structure
 */
#define tqueue_sizeof(t) (sizeof(struct tqueue_node) + sizeof(t))

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize tqueue instance
 *
 * Initializes queue internal data structures and creates an empty queue.
 * Other calls must only be made after tqueue_init.
 *
 * @param q queue instance
 * @return int -1 on error, 0 on success
 *
 * @memberof tqueue
 */
int tqueue_init(struct tqueue *q);

/**
 * @brief gets the current queue length.
 *
 * The Function calls sem_getvalue on the underlying producer consumer lock and
 * subtracts 1 if the queue is NULL terminated.
 *
 * @param q queue instance
 *
 * @exception EINTR interrupted by a signal
 * @return int -1 on error, 0 on success
 *
 * @memberof tqueue
 */
int tqueue_length(struct tqueue *q);

/**
 * @brief appends a node to the end queue
 *
 * Appends a given node to the end of the queue. The underlying memory must be
 * managed by the user. If the given node is NULL, the queue will be
 * NULL-terminated. Appending any other element after the NULL-terminator is
 * undefined.
 *
 * In case of any error, the queue will not be modified.
 *
 * @param q queue instance
 * @param n node to append
 *
 * @exception EOVERFLOW the queue is full
 * @exception EINTR interrupted by a signal
 * @return int -1 on error, 0 on success
 *
 * @memberof tqueue
 */
int tqueue_put_node(struct tqueue *q, struct tqueue_node *n);

/**
 * @brief removes the first node from the queue
 *
 * @param q queue instance
 * @param n memory to write output to
 *
 * @exception EINTR interrupted by a signal
 * @return int -1 on error, 0 on success
 *
 * @memberof tqueue
 */
int tqueue_get_node(struct tqueue *q, struct tqueue_node **n);

/**
 * @brief tries to remove the first node from the queue
 *
 * @param q queue instance
 * @param n memory to write output to
 *
 * @exception EAGAIN the queue doesn't contain an element
 * @exception EINTR interrupted by a signal
 * @return int -1 on error, 0 on success
 *
 * @memberof tqueue
 */
int tqueue_tryget_node(struct tqueue *q, struct tqueue_node **n);

/**
 * @brief destroys the given queue.
 *
 * The function does not remove any elements from the queue and must always be
 * the last operation on a given queue. Calling any other queue function after
 * tqueue_fini is undefined. Calling tqueue_fini multiple times is undefined.
 *
 * @param q queue instance
 *
 * @exception EINVAL the queue is not a valid queue
 * @return int -1 on error, 0 on success
 *
 * @memberof tqueue
 */
int tqueue_fini(struct tqueue *q);

#ifdef __cplusplus
}
#endif

#endif // TQUEUE_H_
