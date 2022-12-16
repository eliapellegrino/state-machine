#include "Knee_Fsm.h"
#include <stdio.h>
#include <unistd.h>
  

int main(void)
{
    float actual_time = 0;
    state_machine_t knee_fsm;
    int ret_code = init_knee_fsm(&knee_fsm, &actual_time);
    if (ret_code == SM_OK) {
        printf("SM Succesfully created!\n");
    }
    while (1) {
        usleep(100000);
        actual_time += 0.1;
        printf("Run state machine \n");
        run_state_machine(&knee_fsm);
        printf("\n\n\n");
    }
    
    return 0;
}

