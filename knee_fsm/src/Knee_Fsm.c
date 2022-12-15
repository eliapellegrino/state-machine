#include "Knee_Fsm.h"
#include <stdio.h>

ControlParametersStateTransitions ctrlParsFSM =
{
    .t_stance_flex = 4,
    .t_stance_ext = 0.5,
    .t_pre_swing = 0.7,
    .t_swing_flex = 1.5,
    .t_swing_ext = 3,
	
};

// STANCE_FLEXION
static void sf_stance_flexion(void ) {
    printf("Stance Flexion \n");
}
static bool gf_stance_flexion(void ) {
    printf("Guard Stance Flexion\n");
    if (*actual_time > ctrlParsFSM.t_stance_flex) {
        return true;
    }
    return false;
}
static void enf_stance_flexion(void ) {
    *actual_time = 0.0;
}
static void exf_stance_flexion(void ) {
    //
}
const state_t stance_flexion = {
    .state_id = STANCE_FLEXION,
    .p_state_function = (StateMachine_StateFunc)sf_stance_flexion,
    .p_guard_function = (StateMachine_GuardFunc)gf_stance_flexion,
    .p_entry_function = (StateMachine_EntryFunc)enf_stance_flexion,
    .p_exit_function =  (StateMachine_ExitFunc)exf_stance_flexion
};

// STANCE_EXTENSION
static void sf_stance_extension(void ) {
    printf("Stance Extension \n");
}
static bool gf_stance_extension(void ) {
    printf("Guard Stance Extension\n");
    if (*actual_time > ctrlParsFSM.t_stance_ext) {
        return true;
    }
    return false;
}
static void enf_stance_extension(void ) {
    //
}
static void exf_stance_extension(void ) {
    //
}
const state_t stance_extension = {
    .state_id = STANCE_EXTENSION,
    .p_state_function = (StateMachine_StateFunc)sf_stance_extension,
    .p_guard_function = (StateMachine_GuardFunc)gf_stance_extension,
    .p_entry_function = (StateMachine_EntryFunc)enf_stance_extension,
    .p_exit_function =  (StateMachine_ExitFunc)exf_stance_extension
};

// PRE_SWING
static void sf_pre_swing(void ) {
    printf("Pre Swing\n");
}
static bool gf_pre_swing(void ) {
    printf("Guard Pre Swing\n");
    if (*actual_time > ctrlParsFSM.t_pre_swing) {
        return true;
    }
    return false;
}
static void enf_pre_swing(void ) {
    //
}
static void exf_pre_swing(void ) {
    //
}
const state_t pre_swing = {
    .state_id = PRE_SWING,
    .p_state_function = (StateMachine_StateFunc)sf_pre_swing,
    .p_guard_function = (StateMachine_GuardFunc)gf_pre_swing,
    .p_entry_function = (StateMachine_EntryFunc)enf_pre_swing,
    .p_exit_function =  (StateMachine_ExitFunc)exf_pre_swing
};

// SWING_FLEXION
static void sf_swing_flexion(void ) {
    printf("Swing Flexion\n");
}
static bool gf_swing_flexion(void ) {
    printf("Guard Swing Flexion\n");
    if (*actual_time > ctrlParsFSM.t_swing_flex) {
        return true;
    }
    return false;
}
static void enf_swing_flexion(void ) {
    //
}
static void exf_swing_flexion(void ) {
    //
}
const state_t swing_flexion = {
    .state_id = SWING_FLEXION,
    .p_state_function = (StateMachine_StateFunc)sf_swing_flexion,
    .p_guard_function = (StateMachine_GuardFunc)gf_swing_flexion,
    .p_entry_function = (StateMachine_EntryFunc)enf_swing_flexion,
    .p_exit_function =  (StateMachine_ExitFunc)exf_swing_flexion
};

// SWING_EXTENSION
static void sf_swing_extension(void ) {
    printf("Swing Extension\n");
}
static bool gf_swing_extension(void ) {
    printf("Guard Swing Extension\n");
    if (*actual_time > ctrlParsFSM.t_swing_ext) {
        return true;
    }
    return false;
}
static void enf_swing_extension(void ) {
    //
}
static void exf_swing_extension(void ) {
    //
}
const state_t swing_extension = {
    .state_id = SWING_EXTENSION,
    .p_state_function = (StateMachine_StateFunc)sf_swing_extension,
    .p_guard_function = (StateMachine_GuardFunc)gf_swing_extension,
    .p_entry_function = (StateMachine_EntryFunc)enf_swing_extension,
    .p_exit_function =  (StateMachine_ExitFunc)exf_swing_extension
};

const state_t gait_states[MAX_NUMBER_STATES] = {
    stance_flexion,
    stance_extension,
    pre_swing,
    swing_extension,
    swing_flexion
};

// This matrix represents if the transition between state_r and state_c is possible (it is always square)
const allowed_trans allowed_transistion_table[MAX_NUMBER_STATES][MAX_NUMBER_STATES] = {
    {EVENT_IGNORED,     ALLOWED,        ALLOWED,        ALLOWED,        CANNOT_HAPPEN   },
    {ALLOWED,           EVENT_IGNORED,  ALLOWED,        ALLOWED,        CANNOT_HAPPEN   },
    {ALLOWED,           CANNOT_HAPPEN,  EVENT_IGNORED,  ALLOWED,        CANNOT_HAPPEN   },
    {ALLOWED,           CANNOT_HAPPEN,  CANNOT_HAPPEN,  EVENT_IGNORED,  ALLOWED         },
    {ALLOWED,           CANNOT_HAPPEN,  CANNOT_HAPPEN,  CANNOT_HAPPEN,  EVENT_IGNORED   },
};

state_machine_t *init_knee_fsm ( float *time ) {
    state_machine_t *knee_fsm = create_state_machine();
    if (init_state_machine(knee_fsm, &stance_flexion, allowed_transistion_table, gait_states, MAX_NUMBER_STATES) == SM_OK) {
        printf("Knee fsm correctly created! \n");
        actual_time = time;
        run_state_machine(knee_fsm);
        return knee_fsm;
    } else {
        printf("Error in creating knee fsm");
    }
    
}