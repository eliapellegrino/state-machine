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
static bool gf_stance_flexion(int previous_state ) {
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

StateMachine_GuardFunc gfs_stance_flexion[MAX_TASKS]= { (StateMachine_GuardFunc)gf_stance_flexion,(StateMachine_GuardFunc)gf_stance_flexion};
StateMachine_EntryFunc enfs_stance_flexion[MAX_TASKS] = { (StateMachine_EntryFunc)enf_stance_flexion,(StateMachine_EntryFunc)enf_stance_flexion};
StateMachine_StateFunc sfs_stance_flexion[MAX_TASKS]= { (StateMachine_StateFunc)sf_stance_flexion, (StateMachine_StateFunc)sf_stance_flexion};
StateMachine_ExitFunc  exfs_stance_flexion[MAX_TASKS] = { (StateMachine_ExitFunc)exf_stance_flexion, (StateMachine_ExitFunc)exf_stance_flexion};
const state_t stance_flexion = {
    .state_id = STANCE_FLEXION,
    .p_state_function = sfs_stance_flexion,
    .p_guard_function = gfs_stance_flexion,
    .p_entry_function = enfs_stance_flexion,
    .p_exit_function =  exfs_stance_flexion
};

// STANCE_EXTENSION
static void sf_stance_extension(void ) {
    printf("Stance Extension \n");
}
static bool gf_stance_extension(int previous_state ) {
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

StateMachine_GuardFunc gfs_stance_extension[MAX_TASKS] = { (StateMachine_GuardFunc)gf_stance_extension,(StateMachine_GuardFunc)gf_stance_extension};
StateMachine_EntryFunc enfs_stance_extension[MAX_TASKS] = { (StateMachine_EntryFunc)enf_stance_extension,(StateMachine_EntryFunc)enf_stance_extension};
StateMachine_StateFunc sfs_stance_extension[MAX_TASKS] = { (StateMachine_StateFunc)sf_stance_extension, (StateMachine_StateFunc)sf_stance_extension};
StateMachine_ExitFunc  exfs_stance_extension[MAX_TASKS] = { (StateMachine_ExitFunc)exf_stance_extension, (StateMachine_ExitFunc)exf_stance_extension};
const state_t stance_extension = {
    .state_id = STANCE_EXTENSION,
    .p_state_function = sfs_stance_extension,
    .p_guard_function = gfs_stance_extension,
    .p_entry_function = enfs_stance_extension,
    .p_exit_function =  exfs_stance_extension
};

// PRE_SWING
static void sf_pre_swing(void ) {
    printf("Pre Swing\n");
}
static bool gf_pre_swing(int previous_state ) {
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
StateMachine_GuardFunc gfs_pre_swing[MAX_TASKS] = { (StateMachine_GuardFunc)gf_pre_swing, (StateMachine_GuardFunc)gf_pre_swing};
StateMachine_EntryFunc enfs_pre_swing[MAX_TASKS] = { (StateMachine_EntryFunc)enf_pre_swing,(StateMachine_EntryFunc)enf_pre_swing};
StateMachine_StateFunc sfs_pre_swing[MAX_TASKS] = { (StateMachine_StateFunc)sf_pre_swing, (StateMachine_StateFunc)sf_pre_swing};
StateMachine_ExitFunc  exfs_pre_swing[MAX_TASKS] = { (StateMachine_ExitFunc)exf_pre_swing, (StateMachine_ExitFunc)exf_pre_swing};
const state_t pre_swing = {
    .state_id = PRE_SWING,
    .p_state_function = sfs_pre_swing,
    .p_guard_function = gfs_pre_swing,
    .p_entry_function = enfs_pre_swing,
    .p_exit_function =  exfs_pre_swing
};

// SWING_FLEXION
static void sf_swing_flexion(void ) {
    printf("Swing Flexion\n");
}
static bool gf_swing_flexion(int previous_state ) {
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
StateMachine_GuardFunc gfs_swing_flexion[MAX_TASKS] = { (StateMachine_GuardFunc)gf_swing_flexion, (StateMachine_GuardFunc)gf_swing_flexion};
StateMachine_EntryFunc enfs_swing_flexion[MAX_TASKS] = { (StateMachine_EntryFunc)enf_swing_flexion,(StateMachine_EntryFunc)enf_swing_flexion};
StateMachine_StateFunc sfs_swing_flexion[MAX_TASKS] = { (StateMachine_StateFunc)sf_swing_flexion, (StateMachine_StateFunc)sf_swing_flexion};
StateMachine_ExitFunc  exfs_swing_flexion[MAX_TASKS] = { (StateMachine_ExitFunc)exf_swing_flexion, (StateMachine_ExitFunc)exf_swing_flexion};
const state_t swing_flexion = {
    .state_id = SWING_FLEXION,
    .p_state_function = sfs_swing_flexion,
    .p_guard_function = gfs_swing_flexion,
    .p_entry_function = enfs_swing_flexion,
    .p_exit_function =  exfs_swing_flexion
};

// SWING_EXTENSION
static void sf_swing_extension(void ) {
    printf("Swing Extension\n");
}
static bool gf_swing_extension(int previous_state ) {
    printf("Guard Swing Extension, coming from %d\n", previous_state);
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
StateMachine_GuardFunc gfs_swing_extension[MAX_TASKS] = { (StateMachine_GuardFunc)gf_swing_extension, (StateMachine_GuardFunc)gf_swing_extension};
StateMachine_EntryFunc enfs_swing_extension[MAX_TASKS] = { (StateMachine_EntryFunc)enf_swing_extension,(StateMachine_EntryFunc)enf_swing_extension};
StateMachine_StateFunc sfs_swing_extension[MAX_TASKS] = { (StateMachine_StateFunc)sf_swing_extension,(StateMachine_StateFunc)sf_swing_extension};
StateMachine_ExitFunc  exfs_swing_extension[MAX_TASKS] = { (StateMachine_ExitFunc)exf_swing_extension, (StateMachine_ExitFunc)exf_swing_extension};
const state_t swing_extension = {
    .state_id = SWING_EXTENSION,
    .p_state_function = sfs_swing_extension,
    .p_guard_function = gfs_swing_extension,
    .p_entry_function = enfs_swing_extension,
    .p_exit_function =  exfs_swing_extension
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

int init_knee_fsm ( state_machine_t *StateMachinePtr, float *time ) {
    if (StateMachinePtr != NULL) {
        if (init_state_machine(StateMachinePtr, (state_t*)&stance_flexion, allowed_transistion_table, gait_states, MAX_NUMBER_STATES, STAIRS_UP) == SM_OK) {
            printf("Knee fsm correctly created! \n");
            actual_time = time;
            run_state_machine(StateMachinePtr);
            return SM_OK;
        } else {
            printf("Error in creating knee fsm");
            return SM_NOK;
        }
    } else {
        printf("StateMachine pointer is null");
        return SM_NOK;
    }
    
    
}