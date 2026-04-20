//
// Created by maith on 2026-04-09.
//

#ifndef LAB_CONTROLLER_H
#define LAB_CONTROLLER_H

#include "actor.h"

typedef struct {
    mailbox_t *mb_this;
    mailbox_t *mb_model;
    mailbox_t *mb_timer;
    mailbox_t *mb_view;
} controller_args_t;

typedef struct {
    bool should_exit;
} controller_state_t;

void *run_controller_actor(void *args);

#endif //LAB_CONTROLLER_H
