// #include "Fault.h"
#include "StateMachine.h"
#include <stdlib.h>
#include <stdio.h>
int change_task_state_machine( state_machine_t *StateMachinePtr, int task) {
    StateMachinePtr->task = task;
    return SM_OK;
}
int run_state_machine( state_machine_t *StateMachinePtr) {
    int state_id = StateMachinePtr->current_state->state_id;

    for (int state = 0; state < StateMachinePtr->max_states; state++) {
        if (StateMachinePtr->allowed_transistion_table[StateMachinePtr->max_states*state_id+state] == ALLOWED) {
            const state_t *tmp_state = &(StateMachinePtr->states[state]);
            if (tmp_state->p_guard_function[StateMachinePtr->task](StateMachinePtr->current_state->state_id)) {
                // Call Exit function of "previous" state
                StateMachinePtr->current_state->p_exit_function[StateMachinePtr->task]();
                // Go to the new state
                StateMachinePtr->current_state = (state_t*)tmp_state;
                // Call entry function
                StateMachinePtr->current_state->p_entry_function[StateMachinePtr->task]();
                // We are in a new state, so we can stop iterating
                break;
            }
        }
    }
    StateMachinePtr->current_state->p_state_function[StateMachinePtr->task]();
    return SM_OK;
}

int init_state_machine( state_machine_t *StateMachinePtr, state_t *initial_state, const allowed_trans *allowed_transistion_table, state_t *states, int max_states, int initial_task) {
    if (StateMachinePtr != NULL) {
        StateMachinePtr->current_state = initial_state;
        StateMachinePtr->allowed_transistion_table = allowed_transistion_table;
        StateMachinePtr->states = states;
        StateMachinePtr->max_states = max_states;
        StateMachinePtr->task = initial_task;
        return SM_OK;
    }

    return SM_NOK;
    
}

state_machine_t *create_state_machine(void) {
    return (state_machine_t*)malloc(sizeof(state_machine_t));
}

void destroy_state_machine(state_machine_t *StateMachinePtr) {
    free(StateMachinePtr);
}
