//
// Created by Hussein Hafid on 2026-04-06.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"
#include "raylib.h"

// --------------------------------------------------------------------------------------
// Configuration
// --------------------------------------------------------------------------------------
#define GRID_WIDTH    32
#define GRID_HEIGHT   20
#define CELL_SIZE     32
#define FPS           60
#define GAME_TICK     0.08f // Seconds between moves

// --------------------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------------------
typedef enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } dir_t;

typedef struct {
    queue_t *snake;
    dir_t    dir;
    int      score;
    bool     can_change_dir;
    bool     is_dead;
} game_t;

// --------------------------------------------------------------------------------------
// Function declarations
// --------------------------------------------------------------------------------------
static void handle_input(game_t *game);
static void update_game(game_t *game, cell_t *apple, float *timer);
static void draw_game(const game_t *game, cell_t apple);
static void reset_game(game_t *game, cell_t *apple);
static bool is_pos_on_snake(queue_t *q, int x, int y, bool skip_head);

// --------------------------------------------------------------------------------------
// Main
// --------------------------------------------------------------------------------------
int main(void) {
    InitWindow(GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE, "Snake 2.0");
    SetTargetFPS(FPS);

    game_t game = { .snake = queue_new() };
    cell_t apple = { .color = RED };
    float move_timer = 0;

    reset_game(&game, &apple);

    while (!WindowShouldClose()) {
        handle_input(&game);
        update_game(&game, &apple, &move_timer);
        draw_game(&game, apple);

        if (game.is_dead && IsKeyPressed(KEY_R)) {
            reset_game(&game, &apple);
        }
    }

    queue_free(game.snake);
    CloseWindow();
    return 0;
}

// --------------------------------------------------------------------------------------
// Function definitions
// --------------------------------------------------------------------------------------
static void reset_game(game_t *game, cell_t *apple) {
    while (game->snake->head) queue_remove(game->snake);

    game->dir = DIR_UP;
    game->score = 0;
    game->is_dead = false;

    cell_t head = { GRID_WIDTH / 2, GRID_HEIGHT / 2, GREEN };
    queue_add(game->snake, head);

    // Place initial apple
    do {
        apple->x = GetRandomValue(0, GRID_WIDTH - 1);
        apple->y = GetRandomValue(0, GRID_HEIGHT - 1);
    } while (is_pos_on_snake(game->snake, apple->x, apple->y, false));
}

static void handle_input(game_t *game) {
    if (game->is_dead || !game->can_change_dir) return;

    dir_t old_dir = game->dir;
    // Direct assignment allows the "last key pressed" to override previous ones per frame
    if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) if (game->dir != DIR_DOWN)  game->dir = DIR_UP;
    if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) if (game->dir != DIR_UP)    game->dir = DIR_DOWN;
    if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) if (game->dir != DIR_RIGHT) game->dir = DIR_LEFT;
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) if (game->dir != DIR_LEFT)  game->dir = DIR_RIGHT;

    if (old_dir != game->dir) {
        game->can_change_dir = false;
    }
}

static void update_game(game_t *game, cell_t *apple, float *timer) {
    if (game->is_dead) return;

    *timer += GetFrameTime();
    if (*timer < GAME_TICK) return;
    *timer = 0;

    cell_t head = game->snake->tail->value;
    int nx = head.x;
    int ny = head.y;

    switch (game->dir) {
        case DIR_UP:    ny--; break;
        case DIR_DOWN:  ny++; break;
        case DIR_LEFT:  nx--; break;
        case DIR_RIGHT: nx++; break;
    }

    // Wrapping logic
    nx = (nx + GRID_WIDTH) % GRID_WIDTH;
    ny = (ny + GRID_HEIGHT) % GRID_HEIGHT;

    // Check Self-Collision
    if (is_pos_on_snake(game->snake, nx, ny, true)) {
        game->is_dead = true;
        return;
    }

    // Check Apple
    bool eating = (nx == apple->x && ny == apple->y);

    // Core Movement: Add new head, remove tail ONLY if not eating
    cell_t new_head = { nx, ny, GREEN };
    queue_add(game->snake, new_head);

    if (eating) {
        game->score++;
        do {
            apple->x = GetRandomValue(0, GRID_WIDTH - 1);
            apple->y = GetRandomValue(0, GRID_HEIGHT - 1);
        } while (is_pos_on_snake(game->snake, apple->x, apple->y, false));
    } else {
        queue_remove(game->snake);
    }

    game->can_change_dir = true;
}

static void draw_game(const game_t *game, cell_t apple) {
    BeginDrawing();
    ClearBackground(BLACK);

    // Draw Apple (with a 1px padding for a cleaner look)
    DrawRectangle(
        apple.x * CELL_SIZE + 1,
        apple.y * CELL_SIZE + 1,
        CELL_SIZE - 2,
        CELL_SIZE - 2,
        apple.color
    );

    // Draw Snake
    node_t *curr = game->snake->head;
    while (curr) {
        DrawRectangle(
            curr->value.x * CELL_SIZE + 1,
            curr->value.y * CELL_SIZE + 1,
            CELL_SIZE - 2,
            CELL_SIZE - 2,
            curr->value.color
        );
        curr = curr->next;
    }

    // Scoreboard
    DrawText(TextFormat("SCORE: %03d", game->score), 10, 10, 20, RAYWHITE);

    if (game->is_dead) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.7f));
        DrawText("GAME OVER", GetScreenWidth()/2 - 70, GetScreenHeight()/2 - 20, 30, RED);
        DrawText("PRESS 'R' TO RESTART", GetScreenWidth()/2 - 70, GetScreenHeight()/2 + 20, 15, RAYWHITE);
    }

    EndDrawing();
}

static bool is_pos_on_snake(queue_t *q, int x, int y, bool skip_head) {
    node_t *curr = skip_head ? q->head->next : q->head;
    while (curr) {
        if (curr->value.x == x && curr->value.y == y) return true;
        curr = curr->next;
    }
    return false;
}