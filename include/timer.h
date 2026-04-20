//
// Created by Hussein Hafid on 2026-04-09.
//

#ifndef LAB_TIMER_H
#define LAB_TIMER_H

#include <stdbool.h>

#include "actor.h"

/**
 * Structure for view arguments.
 * -@code mb_this@endcode: own mailbox
 * -@code mb_model@endcode: model's mailbox
 */
typedef struct {
    mailbox_t *mb_this;
    mailbox_t *mb_model;
} timer_args_t;

/**
 * Structure for timer state.
 * -@code seconds@endcode: time period per tick
 */
typedef struct {
    double seconds;
    bool should_exit;
} timer_state_t;

/**
 * Entry point for the timer actor.
 * @param args make sure to cast this to @code timer_args_t@endcode
 * @return whatever needs to be passed back to the caller (usually nothing hence NULL)
 */
void *run_timer_actor(void *args);

#endif //LAB_TIMER_H
