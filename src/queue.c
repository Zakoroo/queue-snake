//
// Created by Hussein Hafid on 2026-04-06.
//

#include <stdio.h>
#include <stdlib.h>
#include "../include/queue.h"

// ----------------------------------------------------------------------------
// Queue manipulation
// ----------------------------------------------------------------------------
node_t *node_new(void *value, node_t *next) {
    node_t *node = (node_t*) malloc(sizeof(node_t));
    if (node == NULL) return NULL;
    node->value = value;
    node->next = next;
    return node;
}

queue_t *queue_new() {
    queue_t *queue = (queue_t*) malloc(sizeof(queue_t));
    if (queue == NULL) return NULL;
    queue->head = NULL; // No sentinel node
    queue->tail = NULL;
    queue->length = 0;
    return queue;
}

void queue_add(queue_t *queue, void *value) {
    if (queue == NULL) return;

    node_t *node = node_new(value, NULL);

    if (queue->tail == NULL) {
        // List was empty
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->length++;
}

void *queue_peek(queue_t *queue) {
    if (queue == NULL || queue->tail == NULL) return NULL;
    return queue->tail->value;
}

void *queue_remove(queue_t *queue) {
    if (queue == NULL || queue->head == NULL) return NULL;

    node_t *target = queue->head;
    void *value = target->value;

    queue->head = target->next;

    // If we just removed the last node, update the tail to NULL
    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    free(target);
    queue->length--;
    return value;
}

void queue_free(queue_t *queue) {
    if (queue == NULL) return;

    node_t *current = queue->head;
    while (current != NULL) {
        node_t *next = current->next;
        free(current);
        current = next;
    }

    free(queue);
}

void queue_print(queue_t *queue, void (*func_ptr)(void *value)) {
    if (queue == NULL) return;

    node_t *current = queue->head; // Start directly at head
    printf("[");

    while (current != NULL) {
        func_ptr(current->value);
        if (current->next != NULL) {
            printf(",");
        }
        current = current->next;
    }

    printf("]\n");
}
