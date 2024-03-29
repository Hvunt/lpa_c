
#ifndef LPA_H
#define LPA_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "node.h"
#include "priority_queue.h"
#include "list.h"

#define LPA_MAKE_OBSTACLES 1

typedef struct l list_t;

typedef struct path
{
    list_t *current_path;
    int length;
} path_t;


int lpa_init(int x_MAX, int y_MAX);
int lpa_compute_path(int goalX, int goalY);
path_t * lpa_get_path(void);
int lpa_get_path_length(void);
void lpa_get_current_coords(char *data);
node_t lpa_pop_node(void);
void lpa_deinit(void);

void lpa_show_map(int x_goal, int y_goal);

enum
{
    LPA_INIT_OUT_OF_MEMORY = -20,
    LPA_INIT_POINT_ERROR = -10,
    LPA_INIT_POINT_IS_OBSTACLE, // point is a obstacle or a non exist
    LPA_PATH_CANT_BE_FOUND = -1,

    LPA_OK = 1,
};

#endif // LPA_H