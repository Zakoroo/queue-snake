//
// Created by Hussein Hafid on 2026-04-08.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "raylib.h"
#include "common.h"
#include "actor.h"
#include "messages.h"
#include "model.h"
#include "deque.h"

// ---------------------------------------------------------------------------------------------------------------------
// Private functions
// ---------------------------------------------------------------------------------------------------------------------
static void send_view_update(mailbox_t *mb_view, int x, int y, int state, Color color);
static void send_score_update(mailbox_t *mb_view);
static void spawn_apple(Vector2 *apple, deque_t *snake_body);

// ---------------------------------------------------------------------------------------------------------------------
// Entry point for the View Actor
// ---------------------------------------------------------------------------------------------------------------------
void *run_model_actor(void *args) {
    model_args_t *model_args = (model_args_t*) args;
    mailbox_t *mb_this = model_args->mb_this;
    mailbox_t *mb_timer = model_args->mb_timer;
    mailbox_t *mb_view = model_args->mb_view;


    model_state_t state = {
        .snake_dir = DIR_UP, // Assuming DIR_UP is defined in your headers
        .snake_body = deque_new(),
        .should_exit = false
    };

    // Initial snake position
    Vector2 *start_pos = malloc(sizeof(Vector2));
    *start_pos = (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
    deque_add_last(state.snake_body, start_pos);
    send_view_update(mb_view, start_pos->x, start_pos->y, ON, GREEN);

    // Initial apple spawn
    spawn_apple(&state.apple_pos, state.snake_body);
    send_view_update(mb_view, state.apple_pos.x, state.apple_pos.y, ON, RED);

    int flag = 0; // Do not worry about this variable :)
    while (!state.should_exit) {
        actor_msg_t *msg = (actor_msg_t*)mailbox_sync_receive(mb_this);

        switch(msg->type) {
            case MSG_TYPE_TIMER_TICK: {
                flag = 0;
                Vector2 *head = (Vector2*)deque_peek_first(state.snake_body);

                Vector2 *new_head = malloc(sizeof(Vector2));
                *new_head = *head;

                // Move head based on direction
                if (state.snake_dir == DIR_UP)    new_head->y--;
                if (state.snake_dir == DIR_DOWN)  new_head->y++;
                if (state.snake_dir == DIR_LEFT)  new_head->x--;
                if (state.snake_dir == DIR_RIGHT) new_head->x++;

                // Screen wrapping
                if (new_head->x < 0) new_head->x = SCREEN_WIDTH - 1;
                if (new_head->x >= SCREEN_WIDTH) new_head->x = 0;
                if (new_head->y < 0) new_head->y = SCREEN_HEIGHT - 1;
                if (new_head->y >= SCREEN_HEIGHT) new_head->y = 0;

                // Self collision check
                bool collided = false;
                node_t *current = state.snake_body->head->next; // Skip sentinel
                while (current != NULL) {
                    Vector2 *segment = (Vector2*)current->value;
                    if (new_head->x == segment->x && new_head->y == segment->y) {
                        collided = true;
                        break;
                    }
                    current = current->next;
                }

                if (collided) {
                    // Send Game Over instead of Exit to keep View running
                    actor_msg_t *go_msg = malloc(sizeof(actor_msg_t));
                    go_msg->type = MSG_TYPE_GAME_OVER;
                    go_msg->data = NULL;
                    mailbox_send(mb_view, go_msg);

                    state.should_exit = true; // Stop the model actor
                    free(new_head);
                    break;
                }

                deque_add_first(state.snake_body, new_head);
                send_view_update(mb_view, new_head->x, new_head->y, ON, GREEN);

                // Check collision with apple
                if (new_head->x == state.apple_pos.x && new_head->y == state.apple_pos.y) {
                    // Growth: Do NOT remove the tail, just spawn a new apple
                    spawn_apple(&state.apple_pos, state.snake_body);
                    send_view_update(mb_view, state.apple_pos.x, state.apple_pos.y, ON, RED);
                    send_score_update(mb_view);
                } else {
                    // Regular move: Remove tail
                    Vector2 *old_tail = (Vector2*)deque_remove_last(state.snake_body);
                    send_view_update(mb_view, old_tail->x, old_tail->y, OFF, BLACK);
                    free(old_tail);
                }
                break;
            }
            case MSG_TYPE_INPUT: {
                KeyboardKey key = *(KeyboardKey*)msg->data;

                if (flag == 0) {
                    if (key == KEY_W && state.snake_dir != DIR_DOWN) state.snake_dir = DIR_UP;
                    if (key == KEY_S && state.snake_dir != DIR_UP)   state.snake_dir = DIR_DOWN;
                    if (key == KEY_A && state.snake_dir != DIR_RIGHT) state.snake_dir = DIR_LEFT;
                    if (key == KEY_D && state.snake_dir != DIR_LEFT)  state.snake_dir = DIR_RIGHT;
                }

                flag = 1;
                free(msg->data);
                break;
            }
            case MSG_TYPE_EXIT:
                state.should_exit = true;
                break;
            default:
                // Do nothing
        }
        free(msg);
    }

    deque_free(state.snake_body);
    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------
// Private Helpers
// ---------------------------------------------------------------------------------------------------------------------
static void send_view_update(mailbox_t *mb_view, int x, int y, int state, Color color) {
    actor_msg_t *msg = malloc(sizeof(actor_msg_t));
    cmd_render_cell_t *data = malloc(sizeof(cmd_render_cell_t));
    data->point = (Vector2){(float)x, (float)y};
    data->cell = (cell_t){state, color};

    msg->type = MSG_TYPE_VIEW_RENDER_CELL;
    msg->data = data;
    mailbox_send(mb_view, msg);
}

static void send_score_update(mailbox_t *mb_view) {
    actor_msg_t *msg = malloc(sizeof(actor_msg_t));

    msg->type = MSG_TYPE_INC_SCORE;
    msg->data = NULL;
    mailbox_send(mb_view, msg);
}

static void spawn_apple(Vector2 *apple, deque_t *snake_body) {
    int taken_x[SCREEN_WIDTH];
    int taken_y[SCREEN_HEIGHT];
    for (node_t *current = snake_body->head->next; current != NULL; current = current->next) {
        Vector2 *segment = (Vector2*)current->value;
        taken_x[(int)segment->x] = 1;
        taken_y[(int)segment->y] = 1;
    }

    int apple_x;
    do {
         apple_x = GetRandomValue(0, SCREEN_WIDTH - 1);
    } while (taken_x[apple_x] == 1);

    int apple_y;
    do {
         apple_y = GetRandomValue(0, SCREEN_HEIGHT - 1);
    } while (taken_y[apple_y] == 1);

    apple->x = (float) apple_x;
    apple->y = (float) apple_y;
}