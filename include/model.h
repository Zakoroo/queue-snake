//
// Created by Hussein Hafid on 2026-04-09.
//

#ifndef LAB_MODEL_H
#define LAB_MODEL_H

#include <stdbool.h>

#include "actor.h"

/**
 * Structure for model arguments.
 * -@code mb_this@endcode: own mailbox
 * -@code mb_timer@endcode: timer's mailbox
 */
typedef struct {
    mailbox_t *mb_this;
    mailbox_t *mb_timer;
    mailbox_t *mb_view;
} model_args_t;

/**
 * Structure for model state.
 * -@code snake@endcode: a deque storing coordinates to snake segments
 */
typedef struct {
    int snake_dir;
    deque_t *snake_body;
    Vector2 apple_pos;
    bool should_exit;
} model_state_t;

/**
 * Entry point for the model actor.
 * @param args make sure to cast this to @code model_args_t@endcode
 * @return whatever needs to be passed back to the caller (usually nothing hence NULL)
 */
void *run_model_actor(void *args);

#endif //LAB_MODEL_H
