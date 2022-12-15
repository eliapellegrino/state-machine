#ifndef _KNEE_STATE_MACHINE_H
#define _KNEE_STATE_MACHINE_H
/**
 * Little State Machine Implementation.
 * With this StateMachine, each state is described by:
 * - guard action: Wheter or not you can enter the new state
 * - entry action: Perform a specific action when entering
 * - state function: Function to be performed when you are inside the state
 * - exit function: Function to call before exiting the state
 * 
 * The state machine works in this way:
 * 1) Create the SM by calling "create state machine"
 * 2) Initialize the SM byu calling init_state_machine (see below for params)
 * 3) Run the state machine at each loop
*/

#include "DataTypes.h"
// #include "Fault.h"

// Return code for state machine
typedef enum state_machine_ret_code {
    SM_NOK,
    SM_OK
} state_machine_ret_code;

// Enum for transistio between different states
typedef enum allowed_trans{
    EVENT_IGNORED = -1,
    ALLOWED,
    CANNOT_HAPPEN
} allowed_trans;

// Generic state function signatures
typedef void (*StateMachine_StateFunc)(void); // State Function
typedef bool (*StateMachine_GuardFunc)(void); // Guard Function
typedef void (*StateMachine_EntryFunc)(void); // Entry Function
typedef void (*StateMachine_ExitFunc)(void);  // Exit Function

// Description of a state
typedef struct State
{
    int state_id;
    StateMachine_StateFunc    p_state_function;   // State function
    StateMachine_GuardFunc    p_guard_function;   // Guard function
    StateMachine_EntryFunc    p_entry_function;   // Entry function
    StateMachine_ExitFunc     p_exit_function;    // Exit function
} state_t;

// State machine structure
typedef struct StateMachine
{
    state_t *current_state;                             // Current state of the SM
    const allowed_trans *allowed_transistion_table;     // Transistion table (map if from one state you can go to another state)
    const state_t *states;                              // List of possible states
    int max_states;                                     // Maximum number of states
} state_machine_t;

/**
 * run_state_machine: It runs the state machine in this way:
 * 1) Based on the current state, go to the transistion table to see if you can pass to the ALLOWED state.
 * 2) If the guard is true, perform exit_function of the old state, update current state, perform entry function + state_function and return
 * 3) If no transistion is available (all the ALLOWED guards are false), perform the state function of the state
*/
int run_state_machine( state_machine_t *StateMachinePtr);

/**
 * Initialize the state machine
 * @param StateMachinePtr: Pointer to the SM created by create_state_machine
 * @param initial_state: Initial state of the SM
 * @param allowed_transistion_table: Transistion table which map the possibility to pass from one state to another. This must be a matrix of N x N state, where N is the maximum number of state.
 *                                   Here we pass a pointer in order to not be fix to a constant amount of maximum states
 * @param states: Possible states of the state machine, passed as an array of state_t (Here we use pointer to not be fixed to a maximum number of states).
 * @param max_states: Maximum number of states (this could be auto calculated from states)
*/
int init_state_machine( state_machine_t *StateMachinePtr, state_t *initial_state, const allowed_trans *allowed_transistion_table, const state_t *states, int max_states);

/**
 * Allocate the space for a StateMachine on the heap (using malloc)
*/
state_machine_t *create_state_machine( void );
void destroy_state_machine(state_machine_t *StateMachinePtr);
#endif // _KNEE_STATE_MACHINE_H
