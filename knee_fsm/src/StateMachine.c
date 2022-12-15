// #include "Fault.h"
#include "StateMachine.h"
#include <stdlib.h>
#include <stdio.h>

int run_state_machine( state_machine_t *StateMachinePtr) {
    int state_id = StateMachinePtr->current_state->state_id;

    for (int state = 0; state < StateMachinePtr->max_states; state++) {
        if (StateMachinePtr->allowed_transistion_table[StateMachinePtr->max_states*state_id+state] == ALLOWED) {
            const state_t *tmp_state = &(StateMachinePtr->states[state]);
            if (tmp_state->p_guard_function()) {
                // Call Exit function of "previous" state
                StateMachinePtr->current_state->p_exit_function();
                // Go to the new state
                StateMachinePtr->current_state = tmp_state;
                // Call entry function
                StateMachinePtr->current_state->p_entry_function();
                // We are in a new state, so we can stop iterating
                break;
            }
        }
    }
    StateMachinePtr->current_state->p_state_function();
    return SM_OK;
}

int init_state_machine( state_machine_t *StateMachinePtr, state_t *initial_state, const allowed_trans *allowed_transistion_table, const state_t *states, int max_states) {
    if (StateMachinePtr != NULL) {
        StateMachinePtr->current_state = initial_state;
        StateMachinePtr->allowed_transistion_table = allowed_transistion_table;
        StateMachinePtr->states = states;
        StateMachinePtr->max_states = max_states;
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
