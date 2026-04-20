//
// Created by Hussein Hafid on 2026-04-08.
//

#ifndef LAB_MESSAGES_H
#define LAB_MESSAGES_H

#include "raylib.h"
#include "view.h"

typedef enum {
    // timer
    MSG_TYPE_TIMER_TICK,
    MSG_TYPE_TIMER_SET,

    // view
    MSG_TYPE_VIEW_RENDER_CELL,

    // input
    MSG_TYPE_INPUT,
    MSG_TYPE_INC_SCORE,
    MSG_TYPE_GAME_OVER,
    MSG_TYPE_EXIT,
} msg_type_t;

typedef struct {
    msg_type_t type;
    void *data;
} actor_msg_t;

// ---------------------------------------------------------------------------------------------------------------------
// View message structures
// ---------------------------------------------------------------------------------------------------------------------
typedef struct {
    Vector2 point;
    cell_t cell;
} cmd_render_cell_t;

// ---------------------------------------------------------------------------------------------------------------------
// Timer message structures
// ---------------------------------------------------------------------------------------------------------------------
typedef struct {
    double seconds;
} cmd_timer_set_t;


#endif //LAB_MESSAGES_H
