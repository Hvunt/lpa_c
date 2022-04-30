#ifndef LIST_H
#define LIST_H

#include "node.h"
#include <stdlib.h>

struct l
{
    node_t *nodes;
    struct l *next;
};

typedef struct l list_t;

void list_add(list_t **list, node_t *node);
void list_pop(list_t **list);

int list_get_length(list_t *list);
void list_free(list_t **list);

#endif