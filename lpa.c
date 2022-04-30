
#include "lpa.h"

#include <stdio.h>
#include <stdlib.h>

node_t *map;
static int x_current = 0, y_current = 0;

static int x_MAX, y_MAX;

static PQ_list_t *queue = NULL;
static path_t path;

static void get_predecessors(node_t *map, list_t **pred_list, node_t *current_node);
static void get_successors(node_t *map, list_t **suc_list, node_t *current_node);
static node_t *get_min_pred(node_t *map, node_t *current_node);
static void update_node(node_t *map, PQ_list_t **queue, node_t *current_node, node_t *node, node_t *goal_node);
static list_t * make_path(node_t *map, node_t *current_node, node_t *goal);

static void map_init(node_t *map, int x_MAX, int y_MAX);
static node_t *get_node_coord(node_t *map, int x, int y);
static float get_cost(node_t *from, node_t *to);
static void calc_key(float *key, node_t *current_node, node_t *goal_node);

static void print_map(node_t *map, int x_goal, int y_goal);

#ifdef LPA_MAKE_OBSTACLES
static void make_obstacles(void);
#endif // LPA_MAKE_OBSTACLES

// params - is a size of a map
int lpa_init(int _x_MAX, int _y_MAX)
{
    x_MAX = _x_MAX;
    y_MAX = _y_MAX;

    map = (node_t *)malloc(sizeof(node_t) * x_MAX * y_MAX);
    if (map == NULL)
        return LPA_INIT_OUT_OF_MEMORY;

    map_init(map, x_MAX, y_MAX);
    return LPA_OK;
}

void lpa_deinit(void)
{
    x_current = 0;
    y_current = 0;
    free(map);
}

// params is goal coordinates
int lpa_compute_path(int goalX, int goalY)
{
    node_t *current_node = get_node_coord(map, x_current, y_current);

    if (current_node == NULL)
        return LPA_INIT_POINT_ERROR;
    current_node->rhs = 0;

    node_t *goal_node = get_node_coord(map, goalX, goalY);
    if (goal_node == NULL)
        return -1;
    if (current_node->isObstacle || goal_node->isObstacle)
        return LPA_INIT_POINT_IS_OBSTACLE;

    float key_[2];
    calc_key(key_, current_node, goal_node);

    PQ_push(&queue, key_, current_node);
    
    float *top_key = PQ_getTopKey(queue), goal_key[2];
    calc_key(goal_key, goal_node, goal_node);

    while ((top_key[0] < goal_key[0] && top_key[1] < goal_key[1]) || (goal_node->rhs != goal_node->g))
    {
        node_t *node = PQ_pop(&queue);
        node->isVisited = true;
        if (node->g > node->rhs)
        {
            node->g = node->rhs;
            list_t *successors = NULL;
            get_successors(map, &successors, node);
            if (successors != NULL)
            {
                while (successors != NULL)
                {
                    update_node(map, &queue, current_node, successors->nodes, goal_node);
                    list_pop(&successors);
                }
            }
        }
        else
        {
            node->g = INFINITY;
            list_t *successors = NULL;
            get_successors(map, &successors, node);
            if (successors != NULL)
            {
                while (successors != NULL)
                {
                    update_node(map, &queue, current_node, successors->nodes, goal_node);
                    list_pop(&successors);
                }
            }
        }
        top_key = PQ_getTopKey(queue);
        if (top_key == NULL)
        {
            return LPA_PATH_CANT_BE_FOUND;
        }

        calc_key(goal_key, goal_node, goal_node);
    }

    path.current_path = make_path(map, current_node, goal_node);
    path.length = list_get_length(path.current_path);
    
    x_current = goal_node->x;
    y_current = goal_node->y;
    PQ_free(&queue);
    return LPA_OK;
}

void lpa_get_current_coords(char *data)
{
    sprintf(data, "%d:%d", x_current, y_current);
}

// print the whole map
void lpa_show_map(int x_goal, int y_goal)
{
    print_map(map, x_goal, y_goal);
}

// get next node in the path
node_t lpa_pop_node(void)
{
    node_t node = list_pop(&path.current_path);
    path.length -= 1;
    return node;
}

path_t * lpa_get_path(void)
{
    return &path;
}

int lpa_get_path_length(void){
    return path.length;
}


/////////////////////////////////////

/////////////////////////////////////

static void update_node(node_t *map, PQ_list_t **queue, node_t *current_node, node_t *node, node_t *goal_node)
{
    if (node != current_node)
    {
        node->rhs = INFINITY;
        list_t *predecessors = NULL;
        get_predecessors(map, &predecessors, node);
        if (predecessors != NULL || predecessors->nodes != NULL)
        {
            while (predecessors != NULL)
            {
                float min = fmin(node->rhs, predecessors->nodes->g + get_cost(predecessors->nodes, node));
                node->rhs = min;
                list_pop(&predecessors);
            }
        }
    }
    if (PQ_contains(*queue, node))
    {
        PQ_remove(queue, node);
    }
    if (node->g != node->rhs)
    {
        float key[2];
        calc_key(key, node, goal_node);
        PQ_push(queue, key, node);
    }
}

static node_t *get_node_coord(node_t *map, int x, int y)
{
    for (int i = 0; i < x_MAX * y_MAX; i++)
        if (map[i].x == x && map[i].y == y)
            return &map[i];
    return NULL;
}

static void get_predecessors(node_t *map, list_t **pred_list, node_t *from_node)
{
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            if (x != 0 || y != 0)
            {
                node_t *node = get_node_coord(map, from_node->x + x, from_node->y + y);
                if (node != NULL && !node->isObstacle)
                {
                    list_add(pred_list, node);
                }
            }
        }
    }
}

static void get_successors(node_t *map, list_t **suc_list, node_t *from_node)
{
    get_predecessors(map, suc_list, from_node);
}

//get min node by rhs
static node_t *get_min_pred(node_t *map, node_t *from_node)
{
    node_t *min_node = from_node;
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            node_t *temp = get_node_coord(map, from_node->x + x, from_node->y + y);
            if (temp != NULL && !temp->isObstacle && min_node->rhs > temp->rhs)
            {
                min_node = temp;
            }
        }
    }
    return min_node;
}

static list_t * make_path(node_t *map, node_t *current_node, node_t *goal)
{
    list_t *path = NULL;
    node_t *node = get_min_pred(map, goal);
    node_t *prev_node = goal;
    
    list_add(&path, prev_node);
    while (node != current_node)
    {
        list_add(&path, node);
        node->isPath = true;
        prev_node = node;
        node = get_min_pred(map, prev_node);
    }
    return path;
}

//UTILS
static void map_init(node_t *map, int x_MAX, int y_MAX)
{
    int i = 0;
    for (int x = 0; x < x_MAX; x++)
    {
        for (int y = 0; y < y_MAX; y++)
        {
            node_t node;
            node.rhs = INFINITY;
            node.g = INFINITY;
            node.x = x;
            node.y = y;
#ifdef LPA_MAKE_OBSTACLES
            int random = rand() % 100; // isObstacle calculating. Only during development
            node.isObstacle = (random > 10) ? false : true;
#else
            node.isObstacle = false;
#endif // LPA_MAKE_OBSTACLES
            node.isVisited = false;
            node.isPath = false;
            *(map + i) = node;
            i++;
        }
    }

#ifdef LPA_MAKE_OBSTACLES
    make_obstacles();
#endif // LPA_MAKE_OBSTACLES
}

static float get_cost(node_t *from, node_t *to)
{
    if (from->x == to->x || from->y == to->y)
        return 1;
    else
        return sqrt(2);
}

static void calc_key(float *key, node_t *from_node, node_t *goal_node)
{
    key[0] = fmin(from_node->g, from_node->rhs) + Node_getHeuristic(from_node, goal_node);
    key[1] = fmin(from_node->rhs, from_node->g);
}

void print_map(node_t *map, int x_goal, int y_goal)
{
    printf("==========================>Y\n");
    printf("||");
    for (int i = 0; i < x_MAX * y_MAX; i++)
    {
        if (map[i].isObstacle)
            printf("#");            //Print an obstacle
        else if (map[i].x == x_goal && map[i].y == y_goal)
            printf("X");            //Print a goal point
        else if (map[i].x == x_current && map[i].y == y_current)
            printf("S");            //Print a start point
        else if (map[i].isPath)
            printf("O");            //Print if point is in the path
        else
            printf(" ");

        if (map[i].y == y_MAX - 1)
            printf("\n||");
    }
    printf("\n\\/\n");
    printf("X\n");
    printf("###########\n");
}

// static void print_map(node_t *map, node_t *current_node, node_t *goal_node)
// {
//     printf("==========================>Y\n");
//     printf("||");
//     for (int i = 0; i < x_MAX * y_MAX; i++)
//     {
//         if (map[i].isObstacle)
//             printf("#");            //Print an obstacle
//         else if (map[i].x == goal_node->x && map[i].y == goal_node->y)
//             printf("X");            //Print a goal point
//         else if (map[i].x == current_node->x && map[i].y == current_node->y)
//             printf("S");            //Print a start point
//         else if (map[i].isPath)
//             printf("O");            //Print if point is in the path
//         else
//             printf(" ");

//         if (map[i].y == y_MAX - 1)
//             printf("\n||");
//     }
//     printf("\n\\/\n");
//     printf("X\n");
//     printf("###########\n");
// }

#ifdef LPA_MAKE_OBSTACLES

//make artificial obstacles like islands on a map
static void make_obstacles(void)
{
    for (int i = 0; i < x_MAX * y_MAX; i += rand() % 30)
    {
        if (map[i].isObstacle)
        {
            for (int x = -2; x <= 2; x++)
            {
                for (int y = -2; y <= 2; y++)
                {
                    if (x != 0 || y != 0)
                    {
                        node_t *node = get_node_coord(map, map[i].x + x, map[i].y + y);
                        if (node != NULL)
                            node->isObstacle = true;
                    }
                }
            }
        }
    }
}
#endif // LPA_MAKE_OBSTACLES