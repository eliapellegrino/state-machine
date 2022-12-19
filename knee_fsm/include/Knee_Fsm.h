#ifndef _KNEE_STATE_MACHINE_H
#define _KNEE_STATE_MACHINE_H

//#define VERBOSE
#ifdef VERBOSE
    #define PRINT(...)       printf(__VA_ARGS__)
#else
    #define PRINT(...)       
#endif

#include "StateMachine.h"
#include "stateMachineParameters.h"
#include "utils/utilities.h"
/**
 * This implements the knee finite state machine, on top of the Standard Finite State
 * Machine. 
 * The state are:
 * - STANCE FLEXION
 * - STANCE EXTENSION
 * - PRE SWING
 * - SWING FLEXION
 * - SWING EXTENSION
 * while the task are:
 * - WALK
 * - SIT,
 * - STAIRS_UP,
 * - STAIRS_DOWN,
 * - SLOPE_UP,
 * - SLOPE_DOWN,
 * - MAX_NUMBER_TASKS
 * Each state must implements the guard (gf_), entry (enf_), state (sf_) and exit (exf_) for each task:
 * In case the function are equal, you can put in the function_pointer array gfs_<state_name> the same function for each task.
 * In this way, the implementation is the same and it will use the current task to perform the guard/entry/state/exit function
 * function that will be used in the implementation. 
*/

extern char *state_names[MAX_NUMBER_STATES];
extern char *task_names[MAX_NUMBER_TASKS];

// STANCE_FLEXION
static void sf_stance_flexion(void);
static bool gf_stance_flexion(int previous_state);
static void enf_stance_flexion(void);
static void exf_stance_flexion(void);

/**
 * Array of function pointer for guard, entry, state and exit for different task
*/
extern StateMachine_GuardFunc gfs_stance_flexion[MAX_NUMBER_TASKS];
extern StateMachine_EntryFunc enfs_stance_flexion[MAX_NUMBER_TASKS];
extern StateMachine_StateFunc sfs_stance_flexion[MAX_NUMBER_TASKS];
extern StateMachine_ExitFunc exfs_stance_flexion[MAX_NUMBER_TASKS];
extern const state_t stance_flexion;

// STANCE_EXTENSION
static void sf_stance_extension(void);
static bool gf_stance_extension(int previous_state);
static void enf_stance_extension(void);
static void exf_stance_extension(void);
/**
 * Array of function pointer for guard, entry, state and exit for different task
*/
extern StateMachine_GuardFunc gfs_stance_extension[MAX_NUMBER_TASKS];
extern StateMachine_EntryFunc enfs_stance_extension[MAX_NUMBER_TASKS];
extern StateMachine_StateFunc sfs_stance_extension[MAX_NUMBER_TASKS];
extern StateMachine_ExitFunc  exfs_stance_extension[MAX_NUMBER_TASKS];
extern const state_t stance_extension;

// PRE_SWING
static void sf_pre_swing(void);
static bool gf_pre_swing(int previous_state);
static void enf_pre_swing(void);
static void exf_pre_swing(void);
/**
 * Array of function pointer for guard, entry, state and exit for different task
*/
extern StateMachine_GuardFunc gfs_pre_swing[MAX_NUMBER_TASKS];
extern StateMachine_EntryFunc enfs_pre_swing[MAX_NUMBER_TASKS];
extern StateMachine_StateFunc sfs_pre_swing[MAX_NUMBER_TASKS];
extern StateMachine_ExitFunc  exfs_pre_swing[MAX_NUMBER_TASKS];
extern const state_t pre_swing;

// SWING_FLEXION
static void sf_swing_flexion(void);
static bool gf_swing_flexion(int previous_state);
static void enf_swing_flexion(void);
static void exf_swing_flexion(void);
/**
 * Array of function pointer for guard, entry, state and exit for different task
*/
extern StateMachine_GuardFunc gfs_swing_flexion[MAX_NUMBER_TASKS];
extern StateMachine_EntryFunc enfs_swing_flexion[MAX_NUMBER_TASKS];
extern StateMachine_StateFunc sfs_swing_flexion[MAX_NUMBER_TASKS];
extern StateMachine_ExitFunc  exfs_swing_flexion[MAX_NUMBER_TASKS];
extern const state_t swing_flexion;

// SWING_EXTENSION
static void sf_swing_extension(void);
static bool gf_swing_extension(int previous_state);
static void enf_swing_extension(void);
static void exf_swing_extension(void);
/**
 * Array of function pointer for guard, entry, state and exit for different task
*/
extern StateMachine_GuardFunc gfs_swing_extension[MAX_NUMBER_TASKS];
extern StateMachine_EntryFunc enfs_swing_extension[MAX_NUMBER_TASKS];
extern StateMachine_StateFunc sfs_swing_extension[MAX_NUMBER_TASKS];
extern StateMachine_ExitFunc  exfs_swing_extension[MAX_NUMBER_TASKS];
extern const state_t swing_extension;

// API
/**
 * Initialize the state machine
*/
int init_knee_fsm(SystemState initial_state, TaskType initial_task);

/**
 * Change task of the StateMachine
*/
int change_task (TaskType task);

/**
 * Run the StateMachine
*/
int run_fsm ();

/**
 * Reset to the default state of each task
*/
int reset_state(SystemState state);

// Variable declaration
typedef struct {
    // time thresholds FSM
    float t_stance_flex;
    float t_stance_ext;
    float t_pre_swing;
    float t_swing_flex;
    float t_swing_ext;
} ControlParametersStateTransitions_test;
float timer_state, timer_swing, timer_stance, timer_cond_preswing;
extern SystemState initial_state_task[MAX_NUMBER_TASKS];
extern state_machine_t KneeStateMachine;

#endif // _KNEE_STATE_MACHINE_H