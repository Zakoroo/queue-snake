//
// Created by Hussein Hafid on 2026-04-06.
//

#include <stdio.h>
#include <stdlib.h>
#include "../include/queue.h"

// --------------------------------------------------------------------------------------
// Queue manipulation
// --------------------------------------------------------------------------------------
node_t *node_new(cell_t value, node_t *next) {
    // TODO: Implement the function
    node_t *node = (node_t*) malloc(sizeof(node_t));
    node->value = value;
    node->next = next;
    return node;
}

queue_t *queue_new() {
    // TODO: Implement the function
    queue_t *queue = (queue_t*) malloc(sizeof(queue_t));
    queue->head = NULL; // No sentinel node
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void queue_add(queue_t *queue, cell_t value) {
    // TODO: Implement the function
    node_t *node = node_new(value, NULL);

    if (queue->tail == NULL) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->length++;
}

cell_t queue_remove(queue_t *queue) {
    // TODO: Implement the function
    node_t *target = queue->head;
    cell_t value = target->value;
    queue->head = target->next;

    if (queue->head == NULL) {queue->tail = NULL;}

    free(target);
    queue->length--;
    return value;
}

void queue_free(queue_t *queue) {
    // TODO: Implement the function
    while (queue->head) queue_remove(queue);
    free(queue);
}

void queue_print(queue_t *queue) {
    // TODO: Implement the function
    node_t *current = queue->head;
    printf("[");

    while (current != NULL) {
        printf("(%d, %d)", current->value.x, current->value.y);
        if (current->next != NULL) {
            printf(",");
        }
        current = current->next;
    }

    printf("]\n");
}
