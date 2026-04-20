//
// Created by Hussein Hafid on 2026-04-08.
//

#ifndef LAB_ACTOR_H
#define LAB_ACTOR_H

#include <pthread.h>
#include "deque.h"

typedef struct {
    deque_t *messages;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} mailbox_t;

/**
 * Initialize mailbox structure.
 * @param mailbox of type @code mailbox_t@endcode
 */
void mailbox_init(mailbox_t *mailbox);

/**
 * Send message to a mailbox.
 * @param mailbox
 * @param message of type @code void*@endcode
 */
void mailbox_send(mailbox_t *mailbox, void *message);

/**
 * Receive next message from the mailbox queue synchronously (blocking).
 * @param mailbox
 * @return message of type @code void*@endcode
 * @warning This function is blocking and should be used with caution.
 */
void *mailbox_sync_receive(mailbox_t *mailbox);

/**
 * Receive next message from the mailbox queue asynchronously (nonblocking).
 * @param mailbox
 * @return message of type @code void*@endcode (NULL if mailbox is emtpy)
 */
void *mailbox_async_receive(mailbox_t *mailbox);

#endif //LAB_ACTOR_H
