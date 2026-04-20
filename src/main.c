#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "actor.h"
#include "view.h"
#include "timer.h"
#include "controller.h"
#include "model.h"

int main(void) {
    // 1. Initialize all mailboxes
    mailbox_t mb_view, mb_model, mb_timer, mb_controller;
    mailbox_init(&mb_view);
    mailbox_init(&mb_model);
    mailbox_init(&mb_timer);
    mailbox_init(&mb_controller);

    // 2. Prepare arguments for each actor
    view_args_t v_args = { .mb_this = &mb_view, .mb_controller = &mb_controller };
    timer_args_t t_args = { .mb_this = &mb_timer, .mb_model = &mb_model };
    controller_args_t c_args = {
        .mb_this = &mb_controller, .mb_model = &mb_model,
        .mb_timer = &mb_timer, .mb_view = &mb_view
    };
    model_args_t m_args = { .mb_this = &mb_model, .mb_view = &mb_view, .mb_timer = &mb_timer };

    // 3. Spawn background threads
    pthread_t t_thread, c_thread, m_thread;
    pthread_create(&t_thread, NULL, run_timer_actor, &t_args);
    pthread_create(&c_thread, NULL, run_controller_actor, &c_args);
    pthread_create(&m_thread, NULL, run_model_actor, &m_args);

    // 4. Start View on the Main Thread (Raylib requirement)
    run_view_actor(&v_args);

    return 0;
}