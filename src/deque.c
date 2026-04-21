//
// Created by Hussein Hafid on 2026-04-06.
//

#include <stdio.h>
#include <stdlib.h>
#include "../include/deque.h"

// ----------------------------------------------------------------------------
// List manipulation
// ----------------------------------------------------------------------------

node_t *node_new(void *value, node_t *next) {
    node_t *node = (node_t*) malloc(sizeof(node_t));
    if (node == NULL) return NULL; // Good practice to check malloc
    node->value = value;
    node->next = next;
    return node;
}

deque_t *deque_new() {
    deque_t *deque = (deque_t*) malloc(sizeof(deque_t));
    if (deque == NULL) return NULL;
    deque->head = NULL; // No sentinel node
    deque->tail = NULL;
    deque->length = 0;
    return deque;
}

void deque_add_first(deque_t *deque, void *value) {
    if (deque == NULL) return;

    node_t *node = node_new(value, deque->head);
    deque->head = node;

    // If it's the first node being added, tail must also point to it
    if (deque->tail == NULL) {
        deque->tail = node;
    }
    deque->length++;
}

void deque_add_last(deque_t *deque, void *value) {
    if (deque == NULL) return;

    node_t *node = node_new(value, NULL);

    if (deque->tail == NULL) {
        // List was empty
        deque->head = node;
        deque->tail = node;
    } else {
        deque->tail->next = node;
        deque->tail = node;
    }
    deque->length++;
}

void *deque_peek_first(deque_t *deque) {
    if (deque == NULL || deque->head == NULL) return NULL;
    return deque->head->value;
}

void *deque_peek_last(deque_t *deque) {
    if (deque == NULL || deque->tail == NULL) return NULL;
    return deque->tail->value;
}

void *deque_remove_first(deque_t *deque) {
    if (deque == NULL || deque->head == NULL) return NULL;

    node_t *target = deque->head;
    void *value = target->value;

    deque->head = target->next;

    // If we just removed the last node, update the tail to NULL
    if (deque->head == NULL) {
        deque->tail = NULL;
    }

    free(target);
    deque->length--;
    return value;
}

void *deque_remove_last(deque_t *deque) {
    if (deque == NULL || deque->head == NULL) return NULL;

    node_t *target = deque->tail;
    void *value = target->value;

    if (deque->head == deque->tail) {
        // Only one element existed
        deque->head = NULL;
        deque->tail = NULL;
    } else {
        // Must traverse to find the node BEFORE the tail
        node_t *current = deque->head;
        while (current->next != deque->tail) {
            current = current->next;
        }
        current->next = NULL;
        deque->tail = current;
    }

    free(target);
    deque->length--;
    return value;
}

void deque_free(deque_t *deque) {
    if (deque == NULL) return;

    node_t *current = deque->head;
    while (current != NULL) {
        node_t *next = current->next;
        free(current);
        current = next;
    }

    free(deque);
}

void deque_print(deque_t *deque, void (*func_ptr)(void *value)) {
    if (deque == NULL) return;

    node_t *current = deque->head; // Start directly at head
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


