#include "StateMachine.h"
/**
 * This implements the knee finite state machine, on top of the Standard Finite State
 * Machine. 
 * The state are:
 * - STANCE FLEXION
 * - STANCE EXTENSION
 * - PRE SWING
 * - SWING FLEXION
 * - SWING EXTENSION
 * 
 * Each state must implements the guard (gf_), entry (enf_), state (sf_) and exit (exf_)
 * function that will be used in the implementation.
*/
enum gait_state_id {
    STANCE_FLEXION,
    STANCE_EXTENSION,
    PRE_SWING,
    SWING_FLEXION,
    SWING_EXTENSION,
    MAX_NUMBER_STATES
};

enum task_id {
    WALK,
    STAIRS_UP,
    MAX_TASKS
};

// STANCE_FLEXION
static void sf_stance_flexion(void);
static bool gf_stance_flexion(int previous_state);
static void enf_stance_flexion(void);
static void exf_stance_flexion(void);
extern StateMachine_GuardFunc gfs_stance_flexion[MAX_TASKS];
extern StateMachine_EntryFunc enfs_stance_flexion[MAX_TASKS];
extern StateMachine_StateFunc sfs_stance_flexion[MAX_TASKS];
extern StateMachine_ExitFunc exfs_stance_flexion[MAX_TASKS];
extern const state_t stance_flexion;

// STANCE_EXTENSION
static void sf_stance_extension(void);
static bool gf_stance_extension(int previous_state);
static void enf_stance_extension(void);
static void exf_stance_extension(void);
extern StateMachine_GuardFunc gfs_stance_extension[MAX_TASKS];
extern StateMachine_EntryFunc enfs_stance_extension[MAX_TASKS];
extern StateMachine_StateFunc sfs_stance_extension[MAX_TASKS];
extern StateMachine_ExitFunc  exfs_stance_extension[MAX_TASKS];
extern const state_t stance_extension;

// PRE_SWING
static void sf_pre_swing(void);
static bool gf_pre_swing(int previous_state);
static void enf_pre_swing(void);
static void exf_pre_swing(void);
extern StateMachine_GuardFunc gfs_pre_swing[MAX_TASKS];
extern StateMachine_EntryFunc enfs_pre_swing[MAX_TASKS];
extern StateMachine_StateFunc sfs_pre_swing[MAX_TASKS];
extern StateMachine_ExitFunc  exfs_pre_swing[MAX_TASKS];
extern const state_t pre_swing;

// SWING_FLEXION
static void sf_swing_flexion(void);
static bool gf_swing_flexion(int previous_state);
static void enf_swing_flexion(void);
static void exf_swing_flexion(void);
extern StateMachine_GuardFunc gfs_swing_flexion[MAX_TASKS];
extern StateMachine_EntryFunc enfs_swing_flexion[MAX_TASKS];
extern StateMachine_StateFunc sfs_swing_flexion[MAX_TASKS];
extern StateMachine_ExitFunc  exfs_swing_flexion[MAX_TASKS];
extern const state_t swing_flexion;

// SWING_EXTENSION
static void sf_swing_extension(void);
static bool gf_swing_extension(int previous_state);
static void enf_swing_extension(void);
static void exf_swing_extension(void);
extern StateMachine_GuardFunc gfs_swing_extension[MAX_TASKS];
extern StateMachine_EntryFunc enfs_swing_extension[MAX_TASKS];
extern StateMachine_StateFunc sfs_swing_extension[MAX_TASKS];
extern StateMachine_ExitFunc  exfs_swing_extension[MAX_TASKS];
extern const state_t swing_extension;

typedef struct {
    // time thresholds FSM
    float t_stance_flex;
    float t_stance_ext;
    float t_pre_swing;
    float t_swing_flex;
    float t_swing_ext;
} ControlParametersStateTransitions;

float *actual_time;
// API
int init_knee_fsm( state_machine_t *StateMachinePtr, float *time );
