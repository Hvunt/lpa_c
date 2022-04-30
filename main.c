
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "lpa.h"

int main(int argc, char const *argv[]){
    //this is used for obstacles making
    srand(time(NULL));

    int X = 0, Y = 0;
    PQ_list_t *queue = NULL;
    list_t *path = NULL;
    printf("LPA init code: %i\n", lpa_init(200, 200));
    while (1){
        printf("Enter goal X coordinate\n");
        scanf("%d", &X);
        printf("Enter goal Y coordinate\n");
        scanf("%d", &Y);
        printf("Is path was found: %d\n", lpa_compute_path(queue, path, X, Y));
        
        // list_free(&path);
    }

    return 1;
}