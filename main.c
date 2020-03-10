#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>

#include "lpa.h"

int main(int argc, char const *argv[])
{
    // int i = 0;
    // char symb = '|';
    // while (1)
    // {
    //     switch (i)
    //     {
    //     case 1:
    //         symb = '/';
    //         break;
    //     case 2:
    //         symb = '-';
    //         break;
    //     case 3:
    //         symb = '\\';
    //         break;
    //     default:
    //         symb = '|';
    //         i = 0;
    //         break;
    //     }
    //     printf("\r%c", symb);
    //     fflush(stdout);
    //     i++;
    //     sleep(1);
    // }
    
    
    int x_max = 300, y_max = 200;
    lpa_init(x_max, y_max);

    srand(time(NULL));
    
    for(uint8_t j = 0; j < 100; j++){
        PQ_list_t *queue = NULL;
        list_t *path = NULL;
        int goal_x = rand() % x_max;
        int goal_y = rand() % y_max;
        printf("goal x,y: %d, %d\n", goal_x, goal_y);
        printf("result code: %d\n", lpa_compute_path(queue, path, goal_x, goal_y));
        // lpa_free(queue, path);
    }
    // getchar();
   
    return 0;
}
