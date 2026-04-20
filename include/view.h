//
// Created by Hussein Hafid on 2026-04-08.
//

#ifndef LAB_VIEW_H
#define LAB_VIEW_H

#include "raylib.h"
#include "actor.h"
#include "common.h"

/**
 * Structure for view arguments.
 * -@code mb_this@endcode: own mailbox
 */
typedef struct {
    mailbox_t *mb_this;
    mailbox_t *mb_controller;
} view_args_t;

/**
 * Structure for cell state.
 * -@code state@endcode either OFF (0) or ON (1)
 * -@code color@endcode of type Color defined by raylib
 */
typedef struct {
    unsigned int state: 1; // on/off
    Color color;
} cell_t;

/**
 * Structure for managing view state.
 * @code grid@endcode 2D array of type cell_t
 */
typedef struct {
    cell_t grid[SCREEN_HEIGHT][SCREEN_WIDTH];
    bool game_over;
    int score;
    bool should_exit;
} view_state_t;

/**
 * Entry point for the view actor.
 * @param args make sure to cast this to @code view_args_t@endcode
 * @return whatever needs to be passed back to the caller (usually nothing hence NULL)
 */
void *run_view_actor(void *args);

#endif //LAB_VIEW_H
