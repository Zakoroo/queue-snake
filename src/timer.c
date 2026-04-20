//
// Created by Hussein Hafid on 2026-04-09.
//

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "actor.h"
#include "timer.h"
#include "messages.h"


// ---------------------------------------------------------------------------------------------------------------------
// Function declarations (private functions)
// ---------------------------------------------------------------------------------------------------------------------
void handle_message(timer_state_t *state, void *msg);

// ---------------------------------------------------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------------------------------------------------
void *run_timer_actor(void *args) {
    // Fetch arguments
    timer_args_t *timer_args = (timer_args_t*) args;
    mailbox_t *mb_this = timer_args->mb_this;
    mailbox_t *mb_model = timer_args->mb_model;

    // Initial state
    timer_state_t state = {
        .seconds = 0.1,
        .should_exit = false
    };

    while (!state.should_exit) {
        // Fetch & handle all messages
        void *rcv_msg;
        while ((rcv_msg = mailbox_async_receive(mb_this)) != NULL) {
            handle_message(&state, rcv_msg);
        }

        // Send notification to the model
        actor_msg_t *send_msg = (actor_msg_t*) malloc(sizeof(actor_msg_t));
        send_msg->type = MSG_TYPE_TIMER_TICK;
        send_msg->data = NULL;
        mailbox_send(mb_model, send_msg);

        // Wait given time
        WaitTime(state.seconds);
    }

    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------
// Function definitions (private functions)
// ---------------------------------------------------------------------------------------------------------------------
void handle_message(timer_state_t *state, void *msg) {
    if (msg != NULL) {
        actor_msg_t *message = (actor_msg_t*) msg;

        switch (message->type) {
            case MSG_TYPE_EXIT:
                state->should_exit = true;
                break;
            case MSG_TYPE_TIMER_SET:
                cmd_timer_set_t *cmd = message->data;
                state->seconds = cmd->seconds;
                free(cmd);
                break;
            default:
                fprintf(stderr, "[TIMER ACTOR] Error, message type not valid!");
        }

        free(msg);
    }
}
