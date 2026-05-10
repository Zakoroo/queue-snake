//
// Created by Hussein Hafid on 2026-04-06.
//

#ifndef LAB_LIST_H
#define LAB_LIST_H

#include "raylib.h"

/**
 * @brief Represents a single coordinate on the game grid.
 */
typedef struct {
    int x;     /**< Horizontal grid coordinate. */
    int y;     /**< Vertical grid coordinate. */
    Color color; /**< Display color of the cell. */
} cell_t;

/**
 * @brief A node within the linked-list based queue.
 */
typedef struct node_t {
    cell_t value;         /**< The cell data stored in this node. */
    struct node_t *next;  /**< Pointer to the next node in the sequence. */
} node_t;

/**
 * @brief A FIFO (First-In, First-Out) queue structure.
 *
 * Used to manage the snake's body, where the head of the queue represents
 * the tail of the snake, and the tail of the queue represents the head.
 * I know it is kind of confusing, but bear with me.
 */
typedef struct {
    int length;     /**< Total number of elements currently in the queue. */
    node_t *head;   /**< Pointer to the oldest element (front of queue). */
    node_t *tail;   /**< Pointer to the newest element (back of queue). */
} queue_t;

/**
 * @brief Allocates and initializes a new node.
 *
 * @param value The cell data to be stored.
 * @param next  Pointer to the next node in the list.
 * @return node_t* Pointer to the newly allocated node.
 */
node_t *node_new(cell_t value, node_t *next);

/**
 * @brief Initializes a new, empty queue.
 *
 * @return queue_t* A pointer to the heap-allocated queue.
 * @note Must be cleaned up using queue_free().
 */
queue_t *queue_new(void);

/**
 * @brief Appends a new cell to the end (tail) of the queue.
 *
 * In Snake, this is typically used to move the head forward.
 *
 * @param queue Pointer to the target queue.
 * @param value The cell data to add.
 */
void queue_add(queue_t *queue, cell_t value);

/**
 * @brief Removes and returns the element at the front (head) of the queue.
 *
 * In Snake, this is used to "shrink" the tail as the snake moves.
 *
 * @param queue Pointer to the target queue.
 * @return cell_t The value that was stored in the removed node.
 * @warning Ensure queue is not empty before calling.
 */
cell_t queue_remove(queue_t *queue);

/**
 * @brief Logs the queue contents to the console for debugging.
 *
 * @param queue Pointer to the queue to print.
 */
void queue_print(queue_t *queue);

/**
 * @brief Deallocates the queue and all its constituent nodes.
 *
 * @param queue Pointer to the queue to be freed.
 */
void queue_free(queue_t *queue);

#endif //LAB_LIST_H
