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
#include "view.h"

// ---------------------------------------------------------------------------------------------------------------------
// Private functions
// ---------------------------------------------------------------------------------------------------------------------
static void handle_message(view_state_t *state, void *msg);
static void handle_render_cell(view_state_t *state, const cmd_render_cell_t *cmd);
static void render(const view_state_t *state);
static void render_cell(int x, int y, const cell_t *cell);

// ---------------------------------------------------------------------------------------------------------------------
// Entry point for the View Actor
// ---------------------------------------------------------------------------------------------------------------------
void *run_view_actor(void *args) {
    view_args_t *view_args = (view_args_t*) args;
    mailbox_t *mb_this = view_args->mb_this;
    mailbox_t *mb_controller = view_args->mb_controller;

    InitWindow(SCREEN_WIDTH * CELL_SIZE, SCREEN_HEIGHT * CELL_SIZE, "Actor Snake");
    SetTargetFPS(FRAME_RATE);

    view_state_t state = {
        .grid = {0},
        .game_over = false,
        .score = 0,
        .should_exit = false
    };

    while (!state.should_exit) {
        if (WindowShouldClose()) {
            state.should_exit = true;
            actor_msg_t *exit_msg = malloc(sizeof(actor_msg_t));
            exit_msg->type = MSG_TYPE_EXIT;
            exit_msg->data = NULL;
            mailbox_send(mb_controller, exit_msg);
        }

        int key = GetKeyPressed();
        if (key != 0) {
            actor_msg_t *input_msg = malloc(sizeof(actor_msg_t));
            KeyboardKey *data = malloc(sizeof(KeyboardKey));
            *data = (KeyboardKey)key;
            
            input_msg->type = MSG_TYPE_INPUT;
            input_msg->data = data;
            mailbox_send(mb_controller, input_msg);
        }

        void *msg;
        while ((msg = mailbox_async_receive(mb_this)) != NULL) {
            handle_message(&state, msg);
        }

        BeginDrawing();
        {
            ClearBackground(BLACK); // clear screen
            render(&state); // render playground (snake + apple)
            DrawFPS(10, 10); // display framerate

            // display score
            char buff[20];
            sprintf(buff, "score: %d", state.score);
            DrawText(buff, (SCREEN_WIDTH-4)*CELL_SIZE, 10, 21, YELLOW);

            if (state.game_over) {
                int fontSize = 40;
                const char* text = "GAME OVER";
                // Center the text using Raylib utilities
                int textWidth = MeasureText(text, fontSize);
                DrawText(text,
                         (SCREEN_WIDTH * CELL_SIZE)/2 - textWidth/2,
                         (SCREEN_HEIGHT * CELL_SIZE)/2 - fontSize/2,
                         fontSize, RED);
            }
        }
        EndDrawing();
    }

    CloseWindow();
    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------
// Private Helpers
// ---------------------------------------------------------------------------------------------------------------------

static void handle_message(view_state_t *state, void *msg) {
    actor_msg_t *message = (actor_msg_t*) msg;

    switch(message->type) {
        case MSG_TYPE_VIEW_RENDER_CELL:
            handle_render_cell(state, (cmd_render_cell_t*)message->data);
            break;
        case MSG_TYPE_INC_SCORE:
            state->score++;
            break;
        case MSG_TYPE_GAME_OVER:
            state->game_over = true; // Set flag but don't exit
            break;
        case MSG_TYPE_EXIT:
            state->should_exit = true;
            break;
        default:
            fprintf(stderr, "[VIEW ACTOR] Error, message type not valid!");
    }

    free(msg);
}

static void handle_render_cell(view_state_t *state, const cmd_render_cell_t *cmd) {
    if (cmd == NULL) return;

    int x = (int)cmd->point.x;
    int y = (int)cmd->point.y;

    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        state->grid[y][x] = cmd->cell;
    }

    free((void*)cmd);
}

static void render(const view_state_t *state) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            render_cell(x, y, &state->grid[y][x]);
        }
    }
}

static void render_cell(int x, int y, const cell_t *cell) {
    if (cell->state == ON) {
        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, cell->color);
    }
}