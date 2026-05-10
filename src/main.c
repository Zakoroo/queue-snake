// --------------------------------------------------------------------------------------
// Standard libraries
// --------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// --------------------------------------------------------------------------------------
// Local libraries
// --------------------------------------------------------------------------------------
#include "queue.h"
#include "raylib.h"

// --------------------------------------------------------------------------------------
// Macro Definitions
// --------------------------------------------------------------------------------------
#define WINDOW_ROWS 20
#define WINDOW_COLS 32
#define CELL_SIZE 32
#define TARGET_FPS 60

// --------------------------------------------------------------------------------------
// Enumerations
// --------------------------------------------------------------------------------------
typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
} direction_t;

// --------------------------------------------------------------------------------------
// Structures
// --------------------------------------------------------------------------------------
typedef struct {
    direction_t direction;
    queue_t *body;
} snake_t;

typedef cell_t apple_t;

// --------------------------------------------------------------------------------------
// Function declarations
// --------------------------------------------------------------------------------------

cell_t *cell_new(int x, int y, Color color);
void cell_draw(cell_t *cell);
snake_t *snake_new(int x, int y);
void snake_draw(snake_t *snake);
void snake_free(snake_t *snake);
apple_t *apple_new(snake_t *snake);
void place_apple(snake_t *snake, apple_t *apple);
bool check_snake_eat_apple(snake_t *snake, apple_t *apple);
bool check_snake_eat_self(snake_t *snake, int head_x, int head_y);

// --------------------------------------------------------------------------------------
// Main function
// --------------------------------------------------------------------------------------
int main(void) {
    // Initialize window
    InitWindow(WINDOW_COLS*CELL_SIZE, WINDOW_ROWS*CELL_SIZE, "Snake");
    SetTargetFPS(TARGET_FPS);

    // Create new snake
    snake_t *snake = snake_new(WINDOW_COLS/2, WINDOW_ROWS/2);

    // Create and place apple
    apple_t *apple = apple_new(snake);

    // State variables
    bool gameover = false;
    bool quit = false;
    int score = 0;

    // Game loop
    while (!WindowShouldClose()) {
        // ----- Draw stuff ----------------
        BeginDrawing();

        // 1. Clear window
        ClearBackground(BLACK);

        // 2. Draw snake
        snake_draw(snake);

        // 3. Draw apple
        cell_draw(apple);

        // 4. Display score
        char buff[20];
        sprintf(buff, "score: %d", score);
        DrawText(buff, (WINDOW_COLS-4)*CELL_SIZE, 10, 21, YELLOW);

        // 5. Display Gamer Over if it is
        if (gameover == true) {
            int fontSize = 40;
            const char* text = "GAME OVER";

            // Center the text using Raylib utilities
            int textWidth = MeasureText(text, fontSize);
            DrawText(
                text,
                (WINDOW_COLS * CELL_SIZE)/2 - textWidth/2,
                (WINDOW_ROWS * CELL_SIZE)/2 - fontSize/2,
                fontSize,
                RED
            );
        }

        EndDrawing();

        // ----- Read input ----------------
        int key = GetKeyPressed();

        // Update snake direction
        switch (key) {
            case KEY_UP:
            case KEY_W:
                if (snake->direction != DOWN) {
                    snake->direction = UP;
                }
                break;
            case KEY_DOWN:
            case KEY_S:
                if (snake->direction != UP) {
                    snake->direction = DOWN;
                }
                break;
            case KEY_LEFT:
            case KEY_A:
                if (snake->direction != RIGHT) {
                    snake->direction = LEFT;
                }
                break;
            case KEY_RIGHT:
            case KEY_D:
                if (snake->direction != LEFT) {
                    snake->direction = RIGHT;
                }
                break;
            case KEY_Q:
                quit = true;
                break;
            default:
                // No direction change
                break;
        }

        if (quit == true) {
            break;
        }
        if (gameover == true) {
            continue;
        }

        // ----- Update state --------------

        cell_t *head = queue_peek(snake->body);
        int head_x = head->x;
        int head_y = head->y;

        switch (snake->direction) {
            case UP:
                head_y--;
                break;
            case DOWN:
                head_y++;
                break;
            case LEFT:
                head_x--;
                break;
            case RIGHT:
                head_x++;
                break;
            default:
                // Do nothing
                break;
        }

        head_x = ((head_x % WINDOW_COLS) + WINDOW_COLS ) % WINDOW_COLS;
        head_y = ((head_y % WINDOW_ROWS) + WINDOW_ROWS ) % WINDOW_ROWS;

        if (check_snake_eat_apple(snake, apple)) {
            // Grow snake
            cell_t *new_head = cell_new(head_x, head_y, head->color);
            queue_add(snake->body, new_head);

            // Update score and apple
            score++;
            place_apple(snake, apple);
        }

        else if (check_snake_eat_self(snake, head_x, head_y)) {
            gameover = true;
        }

        else {
            // Just move snake
            free(queue_remove(snake->body));
            cell_t *new_head = cell_new(head_x, head_y, head->color);
            queue_add(snake->body, new_head);
        }
        WaitTime(0.08);
    }

    snake_free(snake);
    free(apple);
    return 0;
}

// --------------------------------------------------------------------------------------
// Function definitions
// --------------------------------------------------------------------------------------
cell_t  *cell_new(int x, int y, Color color) {
    cell_t *cell = malloc(sizeof(cell_t));
    cell->x = x;
    cell->y = y;
    cell->color = color;
    return cell;
}

void cell_draw(cell_t *cell) {
    if (cell == NULL) return;

    DrawRectangle(
        cell->x*CELL_SIZE,
        cell->y*CELL_SIZE,
        CELL_SIZE,
        CELL_SIZE,
        cell->color
    );
}

snake_t *snake_new(int x, int y) {
    snake_t *snake = (snake_t *)malloc(sizeof(snake_t));

    snake->direction = UP; // Initial direction
    snake->body = queue_new(); // Emtpy new queue

    // Place snake head on the center
    cell_t *head = cell_new(WINDOW_COLS/2, WINDOW_ROWS/2, GREEN);
    queue_add(snake->body, head);

    return snake;
}

void snake_draw(snake_t *snake) {
    if (snake == NULL) return;

    node_t *segment = snake->body->head;
    while (segment != NULL) {
        cell_t *cell = (cell_t*) segment->value;
        cell_draw(cell);
        segment = segment->next;
    }
}

void snake_free(snake_t *snake) {
    if (snake == NULL) return;
    if (snake->body != NULL) queue_free(snake->body);
    free(snake);
}

apple_t *apple_new(snake_t *snake) {
    apple_t *apple = (apple_t *)malloc(sizeof(apple_t));
    apple->color = RED;

    place_apple(snake, apple);
    return apple;
}

void place_apple(snake_t *snake, apple_t *apple) {
    if (snake == NULL || apple == NULL) return;

    bool unique;
    do {
        // Generate a random position
        unique = true;
        apple->x = GetRandomValue(0, WINDOW_COLS - 1);
        apple->y = GetRandomValue(0, WINDOW_ROWS - 1);

        node_t *segment = snake->body->head;
        while (segment != NULL) {
            cell_t *cell = (cell_t*) segment->value;

            if (cell->x == apple->x && cell->y == apple->y) {
                unique = false;
                break;
            }

            segment = segment->next;
        }
    } while (!unique);
}

bool check_snake_eat_apple(snake_t *snake, apple_t *apple) {
    if (snake == NULL || apple == NULL) return false;

    cell_t *head = queue_peek(snake->body);
    return head->x == apple->x && head->y == apple->y;
}

bool check_snake_eat_self(snake_t *snake, int head_x, int head_y) {
    if (snake == NULL || snake->body == NULL) return false;

    node_t *segment = snake->body->head->next;

    while (segment != NULL) {
        cell_t *cell = (cell_t*) segment->value;

        if (cell->x == head_x && cell->y == head_y) {
            return true;
        }

        segment = segment->next;
    }

    return false;
}
