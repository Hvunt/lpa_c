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
    
    
    int x_max = 80, y_max = 250;
    lpa_init(x_max, y_max);

    srand(time(NULL));
    
    //FOR TESTING
    for(uint8_t j = 0; j < 100; j++){
        PQ_list_t *queue = NULL;
        list_t *path = NULL;

        int error_count = 0;
        int goal_x = 0;
        int goal_y = 0;
        do
        {
            goal_x = rand() % x_max;
            goal_y = rand() % y_max;
            int state = lpa_is_obstacle(goal_x, goal_y);
            if (state)
                error_count++; 
            else break;
        } while (error_count < 20);
        
        if (error_count < 20){
            printf("goal x,y: %d, %d\n", goal_x, goal_y);
            printf("result code: %d\n", lpa_compute_path(queue, path, goal_x, goal_y));
        } else 
            printf("FCKING RANDOM");
        // lpa_free(queue, path);
    }
    // getchar();
   
    return 0;
}
