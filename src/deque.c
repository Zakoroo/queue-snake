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
    node->value = value;
    node->next = next;
    return node;
}

deque_t *deque_new() {
    deque_t *deque = (deque_t*) malloc(sizeof(deque_t));
    node_t *sentinel = node_new(NULL, NULL);
    deque->head = sentinel;
    deque->tail = sentinel;
    deque->length = 0;
    return deque;
}

void deque_add_first(deque_t *deque, void *value) {
    if (deque == NULL) return;

    node_t *node = node_new(value, deque->head->next);
    if (deque->head == deque->tail) {
        deque->tail = node;
    }
    deque->head->next = node;
    deque->length++;
}

void deque_add_last(deque_t *deque, void *value) {
    if (deque == NULL) return;

    node_t *node = node_new(value, NULL);
    deque->tail->next = node;
    deque->tail = node;
    deque->length++;
}

void *deque_peek_first(deque_t *deque) {
    if (deque == NULL) return NULL;
    if (deque->head == deque->tail) return NULL;

    return deque->head->next->value;
}

void *deque_peek_last(deque_t *deque) {
    if (deque == NULL) return NULL;
    if (deque->head == deque->tail) return NULL;

    return deque->tail->value;
}

void *deque_remove_first(deque_t *deque) {
    if (deque == NULL) return NULL;
    if (deque->head == deque->tail) return NULL;

    node_t *target = deque->head->next;
    void *value = target->value;

    if (target == deque->tail) {
        deque->tail = deque->head;
    }

    deque->head->next = target->next;
    free(target);
    deque->length--;
    return value;
}

void *deque_remove_last(deque_t *deque) {
    if (deque == NULL) return NULL;
    if (deque->head == deque->tail) return NULL;

    node_t *before = deque->head;
    node_t *target = before->next;

    while (target->next != NULL) {
        before = target;
        target = target->next;
    }

    void *value = target->value;
    before->next = NULL;
    deque->tail = before;
    free(target);
    deque->length--;

    return value;
}


void deque_free(deque_t *deque) {
    if (deque == NULL) {return;}

    node_t *current = deque->head;
    while (current != NULL) {
        node_t *next = current->next;
        free(current);
        current = next;
    }

    free(deque);
}

// ----------------------------------------------------------------------------
// List representation
// ----------------------------------------------------------------------------

void deque_print(deque_t *deque, void (*func_ptr)(void *value)) {
    // You do not need to change this function!
    if (deque == NULL) {return;}

    node_t *current = deque->head->next;
    printf("[");

    while (current != NULL) {
        func_ptr(current->value);
        if (current->next != NULL) {
            printf(",");
        }
        current = current->next;
    }

    printf("%s\n", "]");
}


