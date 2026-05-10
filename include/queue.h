//
// Created by Hussein Hafid on 2026-04-06.
//

#ifndef LAB_LIST_H
#define LAB_LIST_H

#include "raylib.h"

/**
 * Data struct for storing information about active cells on the screen.
 */
typedef struct {
    int x;
    int y;
    Color color;
} cell_t;


/**
 * List node representing an element in the list.
 */
typedef struct node_t {
    void *value;
    struct node_t *next;
} node_t;

/**
 * List structure pointing to this first (head) and last (tail) element.
 */
typedef struct {
    int length;
    node_t *head;
    node_t *tail;
} queue_t;

/**
 * Create a new node.
 * @param value of type @code void*@endcode
 * @param next pointing to the next element in the list
 */
node_t *node_new(void *value, node_t *next);

/**
 *  Create a new empty list (length == 0)
 *  @return pointer ot the new list
 */
queue_t *queue_new();

/**
 *  Add a new element at the end on the list.
 *  @param queue
 *  @param value of type @code void*@endcode
 */
void queue_add(queue_t *queue, void *value);

/**
 *  Get value at of the element at the given index.
 *  @param queue
 *  @param index
 *  @return object of type @code data_t@endcode containing the value at index
 */
void *queue_peek(queue_t *queue);

/**
 *  Remove the element at the given index.
 *  @param queue
 *  @param index
 *  @return object of type @code data_t@endcode containing the value at index
 */
void *queue_remove(queue_t *queue);

/**
 * Returns the string representation of the list.
 * @param queue
 * @return string representation of @code list@endcode
 */
void queue_print(queue_t *queue, void (*func_ptr)(void *value));

/**
 * Frees a list including all nodes belonging to it.
 * @param queue
 */
void queue_free(queue_t *queue);

#endif //LAB_LIST_H
