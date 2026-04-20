//
// Created by Hussein Hafid on 2026-04-06.
//

#ifndef LAB_LIST_H
#define LAB_LIST_H

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
} deque_t;

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
deque_t *deque_new();

/**
 *  Add a new element at the beginning of the deque.
 *  @param deque
 *  @param value of type @code void*@endcode
 */
void deque_add_first(deque_t *deque, void *value);

/**
 *  Add a new element at the end on the list.
 *  @param deque
 *  @param value of type @code void*@endcode
 */
void deque_add_last(deque_t *deque, void *value);

/**
 *  Get value at of the element at the given index.
 *  @param deque
 *  @param index
 *  @return object of type @code data_t@endcode containing the value at index
 */
void *deque_peek_first(deque_t *deque);

/**
 *  Get value at of the element at the given index.
 *  @param deque
 *  @param index
 *  @return object of type @code data_t@endcode containing the value at index
 */
void *deque_peek_last(deque_t *deque);

/**
 *  Remove the element at the given index.
 *  @param deque
 *  @param index
 *  @return object of type @code data_t@endcode containing the value at index
 */
void *deque_remove_first(deque_t *deque);

/**
 *  Remove the element at the given index.
 *  @param deque
 *  @param index
 *  @return object of type @code data_t@endcode containing the value at index
 */
void *deque_remove_last(deque_t *deque);


/**
 * Returns the string representation of the list.
 * @param deque
 * @return string representation of @code list@endcode
 */
void deque_print(deque_t *deque, void (*func_ptr)(void *value));

/**
 * Frees a list including all nodes belonging to it.
 * @param deque
 */
void deque_free(deque_t *deque);

#endif //LAB_LIST_H
