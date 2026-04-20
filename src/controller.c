//
// Created by Hussein Hafid on 2026-04-09.
//

#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "actor.h"
#include "messages.h"
#include "controller.h"


// ---------------------------------------------------------------------------------------------------------------------
// Function declarations (private functions)
// ---------------------------------------------------------------------------------------------------------------------
void message_handler(controller_state_t *state, void *msg);
void send_exit(mailbox_t *mb_receiver);
void send_input(mailbox_t *mb_receiver, KeyboardKey key);

// ---------------------------------------------------------------------------------------------------------------------
// Entry point
// ---------------------------------------------------------------------------------------------------------------------
void *run_controller_actor(void *args) {
    // Fetch arguments
    controller_args_t *controller_args = (controller_args_t *) args;
    mailbox_t *mb_this = controller_args->mb_this;
    mailbox_t *mb_model = controller_args->mb_model;
    mailbox_t *mb_timer = controller_args->mb_timer;
    mailbox_t *mb_view = controller_args->mb_view;

    // Initial state
    controller_state_t state = {
        .should_exit = false
    };

    while (!state.should_exit) {
        actor_msg_t *msg = (actor_msg_t *) mailbox_sync_receive(mb_this);
        if (msg == NULL) continue;

        switch (msg->type) {
            case MSG_TYPE_EXIT: {
                state.should_exit = true;
            }
            break;

            case MSG_TYPE_INPUT: {
                KeyboardKey key = *(KeyboardKey *) msg->data;
                if (key == KEY_Q) {
                    send_exit(mb_model);
                    send_exit(mb_timer);
                    send_exit(mb_view);
                    state.should_exit = true;
                } else if (key != 0) {
                    send_input(mb_model, key);
                }
                free(msg->data);
            }
            break;

            default: {
                printf("[CONTROLLER ACTOR] Error, message type not valid!");
            }
        }

        free(msg);
    }

    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------
// Function definitions (private functions)
// ---------------------------------------------------------------------------------------------------------------------
void send_exit(mailbox_t *mb_receiver) {
    actor_msg_t *exit_msg_for_model = malloc(sizeof(actor_msg_t));
    exit_msg_for_model->type = MSG_TYPE_EXIT;
    exit_msg_for_model->data = NULL;
    mailbox_send(mb_receiver, exit_msg_for_model);
}

void send_input(mailbox_t *mb_receiver, KeyboardKey key) {
    actor_msg_t *send_msg = (actor_msg_t *) malloc(sizeof(actor_msg_t));
    KeyboardKey *cmd_input = (KeyboardKey *) malloc(sizeof(KeyboardKey));
    *cmd_input = key;
    send_msg->type = MSG_TYPE_INPUT;
    send_msg->data = cmd_input;
    mailbox_send(mb_receiver, send_msg);
}
