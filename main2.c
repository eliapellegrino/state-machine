#include "Knee_Fsm.h"
#include <stdio.h>
#include <unistd.h>
  

int main(void)
{
    float actual_time;
    state_machine_t *knee_fsm = init_knee_fsm(&actual_time);
    if (knee_fsm != NULL) {
        printf("SM Succesfully created!\n");
    }
    while (1) {
        usleep(100000);
        actual_time += 0.1;
        printf("Run state machine \n");
        run_state_machine(knee_fsm);
        printf("\n\n\n");
    }

    destroy_state_machine(knee_fsm);
    
    return 0;
}

