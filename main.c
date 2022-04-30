
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "lpa.h"

int main(int argc, char const *argv[]){
    //this is used for making obstacles
    srand(time(NULL));

    int X = 0, Y = 0;
    path_t *path;

    printf("LPA init code: %i\n", lpa_init(500, 500));
    while (1){
        printf("Enter goal X coordinate\n");
        scanf("%d", &X);
        printf("Enter goal Y coordinate\n");
        scanf("%d", &Y);

        int is_path_found = lpa_compute_path(X, Y);
        printf("Is path was found: %d\n", is_path_found);

        if (is_path_found > 0){
            path = lpa_get_path();
            lpa_show_map(X, Y);
            while (path->length > 0)
            {
                node_t node = lpa_pop_node();
                printf("X: %d; Y: %d\n", node.x, node.y);
            }
        }
    }
    lpa_deinit();
    return 1;
}