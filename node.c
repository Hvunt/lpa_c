#include "node.h"
#include <stdlib.h>
#include <math.h>

int Node_isConsistent(node_t *node){
    return node->g == node->rhs ? 1 : 0;
}

float Node_getHeuristic(node_t *current_node, node_t *goal_node){
    return sqrt(pow(abs(current_node->x - goal_node->x), 2) + pow(abs(current_node->y - goal_node->y), 2));
}