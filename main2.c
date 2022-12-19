#include "Knee_Fsm.h"
#include <stdio.h>
#include <unistd.h>

int main(int args, char **argv)
{   
    float actual_time = 0;
    int ret_code = init_knee_fsm(STANCE_FLEXION, WALK);
    if (ret_code == SM_OK) {
        printf("SM Succesfully created!\n");
    }
    while (1) {
        delayMicrosec(5000);
        actual_time += 0.005;
        run_fsm();
        if (actual_time > 4) {
            change_task(SLOPE_UP);
            actual_time = 0;
        }
        printf("\n\n\n");
    }
    
    return 0;
}

