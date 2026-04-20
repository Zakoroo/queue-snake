//
// Created by Hussein Hafid on 2026-04-08.
//

#include <pthread.h>
#include "actor.h"

void mailbox_init(mailbox_t *mailbox) {
    mailbox->messages = deque_new();
    pthread_mutex_init(&mailbox->mutex, NULL);
    pthread_cond_init(&mailbox->cond, NULL);
}

void mailbox_send(mailbox_t *mailbox, void *msg) {
    pthread_mutex_lock(&mailbox->mutex);

    // critical section (begin)
    deque_add_last(mailbox->messages, msg);
    // critical section (end)

    pthread_cond_signal(&mailbox->cond);
    pthread_mutex_unlock(&mailbox->mutex);
}

void *mailbox_sync_receive(mailbox_t *mailbox) {
    pthread_mutex_lock(&mailbox->mutex);
    while (mailbox->messages->length == 0) { // handle spurious wakeups
        pthread_cond_wait(&mailbox->cond, &mailbox->mutex);
    }

    // critical section (begin)
    void *msg = deque_remove_first(mailbox->messages);
    // critical section (end)

    pthread_mutex_unlock(&mailbox->mutex);
    return msg;
}

void *mailbox_async_receive(mailbox_t *mailbox) {
    pthread_mutex_lock(&mailbox->mutex);

    // critical section (begin)
    void *msg = NULL;
    if (mailbox->messages->length != 0) {
        msg = deque_remove_first(mailbox->messages);
    }
    // critical section (end)

    pthread_mutex_unlock(&mailbox->mutex);
    return msg;
}
