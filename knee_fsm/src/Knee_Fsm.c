#include "Knee_Fsm.h"
#include <stdio.h>
#include <math.h>

extern char *state_names[MAX_NUMBER_STATES] = {
    "STANCE_FLEXION",
    "STANCE_EXTENSION",
    "PRE_SWING",
    "SWING_FLEXION",
    "SWING_EXTENSION"
};

extern char *task_names[MAX_NUMBER_TASKS] = {
    "WALK",
    "STAIRS_UP",
    "STAIRS_DOWN",
    "SLOPE_UP",
    "SLOPE_DOWN",
    "SIT"
};

SystemState initial_state_task[MAX_NUMBER_TASKS] = {
    STANCE_FLEXION,
    STANCE_FLEXION,
    STANCE_FLEXION,
    STANCE_FLEXION,
    STANCE_FLEXION,
    STANCE_FLEXION
};

ControlParametersStateTransitions_test ctrlParsFSM_test =
{
    .t_stance_flex = 4,
    .t_stance_ext = 0.5,
    .t_pre_swing = 0.7,
    .t_swing_flex = 1.5,
    .t_swing_ext = 3,
	
};

ControlParametersStateTransitions ctrlParsFSM =
{
    .t_MinState = 0.05f,
	.t_MinCondLock = 0.125f,
	.t_MinCondPreSwing = 0.15f,
    .t_MinStanceFlex = 0.15f,
    .t_MinStand = 0.30f, // increased 0.2 -> 0.3
    .t_MinStance = 0.35f, // increased 0.25 ->0.35
    .t_Fallback = 1.0f,

    .p_SwitchExt = 15.0f / 180.0f * M_PI,
    .p_MinSwingExt = 30.0f / 180.0f * M_PI,
	.pt_MinPreSwing = 0.0f / 180.0f * M_PI,
	.p_Fallback = 20.0f / 180.0f * M_PI,
    .pt_MinSeated = 75.0f / 180.0f * M_PI,

    .F_Hysteresis = 0.025f, // F delta hysteresis
	.F_SwitchContact = 0.1f,
	.F_MaxLift = -0.75f,  // MIGHT BE REMOVED
    .F_Fallback = 0.2f,
	.F_SwitchSupport = 0.3f,
	.F_M_LimitAutocalib = 0.2f, // was general purpose 4

    .M_Hysteresis = 0.1f,
	.M_MinPreSwing = 0.75f,
    .M_Fallback = 0.5f,
	.Md_LimitAutocalib = 0.5f // was general purpose 4
};

ControlInputs ctrlIn = {
    .p_knee = 0.0f,
    .w_knee = 0.0f,
    .w_knee_filtfast = 0.0f,
    .w_knee_filtnormal = 0.0f,
    .w_knee_filtslow = 0.0f,
    .p_shank = 0.0f,
    .w_shank = 0.0f,
    .p_thigh = 0.0f,
    .w_thigh = 0.0f,
    .w_thigh_filtslow = 0.0f,
    .p_motor = 0.0f,
    .w_motor = 0.0f,
    .w_motor_filtfast = 0.0f,
    .w_motor_filtnormal = 0.0f,
    .w_motor_filtslow = 0.0f,
    .F = 0.0f,
    .F_filtmoving = 0.0f,
    .F_autocalib = 0.0f,
    .M = 0.0f,
    .M_autocalib = 0.0f,
    .Md = 0.0f,
    .Md_filtnormal = 0.0f
};

ControlOutputs ctrlOut = {
    .actuation_flag = 1, // must be 1 initially because we start seated, activating the motor.
    .state = STANCE_EXTENSION,
    .vel_ambulation = 0.0f,
    .task = WALK
};

ControlParametersGlobal ctrlParsGlobal = {
    .p_Hysteresis = 0.0f / 180.0f * M_PI, // TO BE USED by angle-based switches
    .p_ExtLimit = 15.0f / 180.0f * M_PI,
    .p_FlexLimit = 70.0f / 180.0f * M_PI,
    .p_FlexResist = 30.0f / 180.0f * M_PI,

    .p_FlexLimitDescent = 20.0f / 180.0f * M_PI,
    .p_Activate = 45.0f / 180.0f * M_PI, // MIGHT BE MOVED TO FSM STRUCT

    .w_Hysteresis = 1.5f / 30.0f * M_PI,
    .w_Motion = 6.0f / 30.0f * M_PI,
};

DeviceParameters devicePars = {
    .xvu_closure = 0.15f, 
    .xvo_closure = 0.15f,
    .p_knee_offset = 0.0f,
    .p_motor_offset = 0.0f,
    .M_offset = 0.0f,
    .F_offset = 0.0f,
};

/*
This is a multidimensional (the depth is the MAXIMUM NUMBER OF TASKS) square matrix,
that represent which transistion is possible from state A to state B. 
Each "depth" represents the task: you  must follow the EXACT ORDER OF @enum TaskType
The StateMachine will control this transistion table everytime run() is called, and if possible
it will perform the transistion from one state to the other one

*/
const allowed_trans allowed_transistion_table[MAX_NUMBER_TASKS][MAX_NUMBER_STATES][MAX_NUMBER_STATES] = {
        // Stance Flexion   Stance Extension    Pre swing       Swing Flexion       Swing Extension
    {
        // Walk
        {EVENT_IGNORED,     ALLOWED,            ALLOWED,        ALLOWED,            CANNOT_HAPPEN   },  // Stance Flexion
        {ALLOWED,           EVENT_IGNORED,      ALLOWED,        ALLOWED,            CANNOT_HAPPEN   },  // Stance Extension
        {ALLOWED,           CANNOT_HAPPEN,      EVENT_IGNORED,  ALLOWED,            CANNOT_HAPPEN   },  // Pre Swing
        {ALLOWED,           CANNOT_HAPPEN,      CANNOT_HAPPEN,  EVENT_IGNORED,      ALLOWED         },  // Swing Flexion
        {ALLOWED,           CANNOT_HAPPEN,      CANNOT_HAPPEN,  CANNOT_HAPPEN,      EVENT_IGNORED   },  // Swing Extension
    },
    {
        // Sit
        {EVENT_IGNORED,     ALLOWED,            CANNOT_HAPPEN,  ALLOWED,            CANNOT_HAPPEN   },  // Stance Flexion
        {ALLOWED,           EVENT_IGNORED,      CANNOT_HAPPEN,  ALLOWED,            CANNOT_HAPPEN   },  // Stance Extension
        {CANNOT_HAPPEN,     CANNOT_HAPPEN,      EVENT_IGNORED,  CANNOT_HAPPEN,      CANNOT_HAPPEN   },  // Pre Swing
        {ALLOWED,           CANNOT_HAPPEN,      CANNOT_HAPPEN,  EVENT_IGNORED,      ALLOWED         },  // Swing Flexion
        {ALLOWED,           CANNOT_HAPPEN,      CANNOT_HAPPEN,  ALLOWED,            EVENT_IGNORED   },  // Swing Extension

    },
    {
        // Stairs Up
        {EVENT_IGNORED,     ALLOWED,            CANNOT_HAPPEN,  ALLOWED,            CANNOT_HAPPEN   },  // Stance Flexion
        {ALLOWED,           EVENT_IGNORED,      ALLOWED,        ALLOWED,            CANNOT_HAPPEN   },  // Stance Extension
        {ALLOWED,           CANNOT_HAPPEN,      EVENT_IGNORED,  ALLOWED,            CANNOT_HAPPEN   },  // Pre Swing
        {ALLOWED,           CANNOT_HAPPEN,      CANNOT_HAPPEN,  EVENT_IGNORED,      ALLOWED         },  // Swing Flexion
        {ALLOWED,           CANNOT_HAPPEN,      CANNOT_HAPPEN,  CANNOT_HAPPEN,      EVENT_IGNORED   },  // Swing Extension
    },
    {
        // Stairs down
        {EVENT_IGNORED,     ALLOWED,            CANNOT_HAPPEN,  ALLOWED,            CANNOT_HAPPEN   },  // Stance Flexion
        {ALLOWED,           EVENT_IGNORED,      CANNOT_HAPPEN,  ALLOWED,            CANNOT_HAPPEN   },  // Stance Extension
        {CANNOT_HAPPEN,     CANNOT_HAPPEN,      EVENT_IGNORED,  CANNOT_HAPPEN,      CANNOT_HAPPEN   },  // Pre Swing
        {ALLOWED,           CANNOT_HAPPEN,      CANNOT_HAPPEN,  EVENT_IGNORED,      ALLOWED         },  // Swing Flexion
        {ALLOWED,           CANNOT_HAPPEN,      CANNOT_HAPPEN,  CANNOT_HAPPEN,      EVENT_IGNORED   },  // Swing Extension
    },
    {
        // Slope up
        {EVENT_IGNORED,     ALLOWED,            ALLOWED,        ALLOWED,            CANNOT_HAPPEN   },  // Stance Flexion
        {ALLOWED,           EVENT_IGNORED,      ALLOWED,        ALLOWED,            CANNOT_HAPPEN   },  // Stance Extension
        {ALLOWED,           CANNOT_HAPPEN,      EVENT_IGNORED,  ALLOWED,            CANNOT_HAPPEN   },  // Pre Swing
        {ALLOWED,           CANNOT_HAPPEN,      CANNOT_HAPPEN,  EVENT_IGNORED,      ALLOWED         },  // Swing Flexion
        {ALLOWED,           CANNOT_HAPPEN,      CANNOT_HAPPEN,  CANNOT_HAPPEN,      EVENT_IGNORED   },  // Swing Extension
    },
    {
        // Slope down
        {EVENT_IGNORED,     ALLOWED,            ALLOWED,        ALLOWED,            CANNOT_HAPPEN   },  // Stance Flexion
        {ALLOWED,           EVENT_IGNORED,      ALLOWED,        ALLOWED,            CANNOT_HAPPEN   },  // Stance Extension
        {ALLOWED,           CANNOT_HAPPEN,      EVENT_IGNORED,  ALLOWED,            CANNOT_HAPPEN   },  // Pre Swing
        {ALLOWED,           CANNOT_HAPPEN,      CANNOT_HAPPEN,  EVENT_IGNORED,      ALLOWED         },  // Swing Flexion
        {ALLOWED,           CANNOT_HAPPEN,      CANNOT_HAPPEN,  CANNOT_HAPPEN,      EVENT_IGNORED   },  // Swing Extension
    }
    
};

state_machine_t KneeStateMachine;

// STANCE_FLEXION
static void sf_stance_flexion(void ) {
    // increase the timers
    timer_stance += TD; // add to stance timer

    // Conditional timer for activating Lock mode
    if (fabsf(ctrlIn.w_knee_filtslow) < ctrlParsGlobal.w_Hysteresis &&
    fabsf(ctrlIn.w_thigh_filtslow) < ctrlParsGlobal.w_Hysteresis &&
    ctrlIn.F_autocalib > ctrlParsFSM.F_Fallback)
    {
    ctrlOut.timer_cond_lock += TD;
    }
    else if (ctrlOut.timer_cond_lock > ctrlParsFSM.t_MinCondLock &&
    ctrlIn.F_autocalib > ctrlParsFSM.F_Fallback &&
    ctrlIn.w_knee_filtslow > -ctrlParsGlobal.w_Hysteresis)
    {
    // stay in lock for this condition
    } else {
    ctrlOut.timer_cond_lock = 0.0f;
    }

    // Conditional timer for switching to preswing
    if (ctrlIn.p_knee < ctrlParsFSM.p_SwitchExt)
    {

        timer_cond_preswing += TD;
        timer_state += TD;

    }
    else
    {

        timer_cond_preswing = 0.0f;
        timer_state = 0.0f;

    }
}
static bool gf_stance_flexion(int previous_state ) {
    bool ret_code = false;
    PRINT("Guard Stance Flexion\n");
    switch (previous_state) {
        case SWING_EXTENSION:
        {
            ret_code =  ctrlIn.F_filtmoving > ctrlParsFSM.F_SwitchContact &&
                        timer_state > ctrlParsFSM.t_MinState;
        } break;

        case SWING_FLEXION:
        {
            ret_code =  ctrlIn.F_filtmoving > ctrlParsFSM.F_SwitchContact &&
                        timer_state > ctrlParsFSM.t_MinState &&
                        ctrlIn.p_knee < ctrlParsFSM.p_Fallback &&
                        (ctrlIn.p_knee < ctrlParsFSM.p_SwitchExt ||
                        timer_swing > ctrlParsFSM.t_Fallback);
        } break;
        
        case PRE_SWING:
        {
            ret_code =  (((ctrlIn.F_filtmoving > ctrlParsFSM.F_Fallback) &&
                        ctrlIn.p_knee > ctrlParsFSM.p_SwitchExt &&
                        ctrlIn.M_autocalib < ctrlParsFSM.M_Fallback) ||
                        // go back to StF anyway after timer, unless you would go back into PSw immediately:
                        ((timer_state > ctrlParsFSM.t_Fallback) &&
                        !(timer_cond_preswing > ctrlParsFSM.t_MinCondPreSwing &&
                        ctrlIn.M_autocalib > ctrlParsFSM.M_MinPreSwing &&
                        timer_stance > ctrlParsFSM.t_MinStand &&
                        ctrlIn.p_thigh > ctrlParsFSM.pt_MinPreSwing)));
        } break;

        case STANCE_EXTENSION :
        {
            ret_code =  (ctrlIn.w_knee_filtfast > ctrlParsGlobal.w_Motion ||
                        (ctrlIn.w_knee_filtfast > ctrlParsGlobal.w_Hysteresis &&
                        timer_state > ctrlParsFSM.t_MinState));
        } break;
    }
    return ret_code;
}
static void enf_stance_flexion(void ) {
    timer_state = 0.0f;
    timer_swing = 0.0f;
    ctrlOut.state = STANCE_FLEXION; 
}
static void exf_stance_flexion(void ) {
    //
}

StateMachine_GuardFunc gfs_stance_flexion[MAX_NUMBER_TASKS]= { 
    (StateMachine_GuardFunc)gf_stance_flexion,
    (StateMachine_GuardFunc)gf_stance_flexion,
    (StateMachine_GuardFunc)gf_stance_flexion,
    (StateMachine_GuardFunc)gf_stance_flexion,
    (StateMachine_GuardFunc)gf_stance_flexion,
    (StateMachine_GuardFunc)gf_stance_flexion
};
StateMachine_EntryFunc enfs_stance_flexion[MAX_NUMBER_TASKS] = {
    (StateMachine_EntryFunc)enf_stance_flexion,
    (StateMachine_EntryFunc)enf_stance_flexion,
    (StateMachine_EntryFunc)enf_stance_flexion,
    (StateMachine_EntryFunc)enf_stance_flexion,
    (StateMachine_EntryFunc)enf_stance_flexion,
    (StateMachine_EntryFunc)enf_stance_flexion
};
StateMachine_StateFunc sfs_stance_flexion[MAX_NUMBER_TASKS]= {
    (StateMachine_StateFunc)sf_stance_flexion,
    (StateMachine_StateFunc)sf_stance_flexion,
    (StateMachine_StateFunc)sf_stance_flexion,
    (StateMachine_StateFunc)sf_stance_flexion,
    (StateMachine_StateFunc)sf_stance_flexion,
    (StateMachine_StateFunc)sf_stance_flexion
};
StateMachine_ExitFunc  exfs_stance_flexion[MAX_NUMBER_TASKS] = {
    (StateMachine_ExitFunc)exf_stance_flexion,
    (StateMachine_ExitFunc)exf_stance_flexion,
    (StateMachine_ExitFunc)exf_stance_flexion,
    (StateMachine_ExitFunc)exf_stance_flexion,
    (StateMachine_ExitFunc)exf_stance_flexion,
    (StateMachine_ExitFunc)exf_stance_flexion
};

const state_t stance_flexion = {
    .state_id = STANCE_FLEXION,
    .p_state_function = sfs_stance_flexion,
    .p_guard_function = gfs_stance_flexion,
    .p_entry_function = enfs_stance_flexion,
    .p_exit_function =  exfs_stance_flexion
};

// STANCE_EXTENSION
static void sf_stance_extension(void ) {
    // increase the timers
    timer_stance += TD; // add to stance timer

    // Conditional timer for activating Lock mode
    if (fabsf(ctrlIn.w_knee_filtslow) < ctrlParsGlobal.w_Hysteresis &&
        fabsf(ctrlIn.w_thigh_filtslow) < ctrlParsGlobal.w_Hysteresis &&
        ctrlIn.F_autocalib > ctrlParsFSM.F_Fallback)
    {
        ctrlOut.timer_cond_lock += TD;
    }
    else if (   ctrlOut.timer_cond_lock > ctrlParsFSM.t_MinCondLock &&
                ctrlIn.F_autocalib > ctrlParsFSM.F_Fallback &&
                ctrlIn.w_knee_filtslow > -ctrlParsGlobal.w_Hysteresis)
    {
        // stay in lock for this condition
    }
    else
    {
        ctrlOut.timer_cond_lock = 0.0f;
    }

    // Conditional timer for switching to preswing
    if (ctrlIn.p_knee < ctrlParsFSM.p_SwitchExt)
    {
        timer_cond_preswing += TD;
    }
    else
    {
        timer_cond_preswing = 0.0f;
    }
    // Conditional timer for switching to stance flexion
    if (ctrlIn.p_knee > ctrlParsFSM.p_SwitchExt)
    {
        timer_state += TD;
    }
    else
    {
        timer_state = 0.0f;
    }
}
static bool gf_stance_extension(int previous_state ) {
    PRINT("Guard Stance Extension\n");
    bool ret_code = false;

    ret_code = ctrlIn.w_knee_filtfast < -ctrlParsGlobal.w_Hysteresis && 
                timer_state > ctrlParsFSM.t_MinState && 
                timer_stance >ctrlParsFSM.t_MinStanceFlex;

    return ret_code;
}
static void enf_stance_extension(void ) {
    ctrlOut.state = STANCE_EXTENSION;
	timer_state = 0;
}
static void exf_stance_extension(void ) {
    //
}

StateMachine_GuardFunc gfs_stance_extension[MAX_NUMBER_TASKS] = {
    (StateMachine_GuardFunc)gf_stance_extension,
    (StateMachine_GuardFunc)gf_stance_extension,
    (StateMachine_GuardFunc)gf_stance_extension,
    (StateMachine_GuardFunc)gf_stance_extension,
    (StateMachine_GuardFunc)gf_stance_extension,
    (StateMachine_GuardFunc)gf_stance_extension
};
StateMachine_EntryFunc enfs_stance_extension[MAX_NUMBER_TASKS] = {
    (StateMachine_EntryFunc)enf_stance_extension,
    (StateMachine_EntryFunc)enf_stance_extension,
    (StateMachine_EntryFunc)enf_stance_extension,
    (StateMachine_EntryFunc)enf_stance_extension,
    (StateMachine_EntryFunc)enf_stance_extension,
    (StateMachine_EntryFunc)enf_stance_extension
};
StateMachine_StateFunc sfs_stance_extension[MAX_NUMBER_TASKS] = {
    (StateMachine_StateFunc)sf_stance_extension,
    (StateMachine_StateFunc)sf_stance_extension,
    (StateMachine_StateFunc)sf_stance_extension,
    (StateMachine_StateFunc)sf_stance_extension,
    (StateMachine_StateFunc)sf_stance_extension,
    (StateMachine_StateFunc)sf_stance_extension
};
StateMachine_ExitFunc  exfs_stance_extension[MAX_NUMBER_TASKS] = {
    (StateMachine_ExitFunc)exf_stance_extension,
    (StateMachine_ExitFunc)exf_stance_extension,
    (StateMachine_ExitFunc)exf_stance_extension,
    (StateMachine_ExitFunc)exf_stance_extension,
    (StateMachine_ExitFunc)exf_stance_extension,
    (StateMachine_ExitFunc)exf_stance_extension
};
const state_t stance_extension = {
    .state_id = STANCE_EXTENSION,
    .p_state_function = sfs_stance_extension,
    .p_guard_function = gfs_stance_extension,
    .p_entry_function = enfs_stance_extension,
    .p_exit_function =  exfs_stance_extension
};

// PRE_SWING
static void sf_pre_swing(void ) {
    // timers
    timer_stance += TD; // add to stance timer
    timer_state += TD; // add to custom timer

    // Conditional timer for switching to PreSwing
    if (ctrlIn.p_knee < ctrlParsFSM.p_SwitchExt)
    {
        timer_cond_preswing += TD;
    }
    else
    {
        timer_cond_preswing = 0.0f;
    }
}
static bool gf_pre_swing(int previous_state ) {
    PRINT("Guard Pre Swing\n");
    bool ret_code = false;
    ret_code =  timer_cond_preswing > ctrlParsFSM.t_MinCondPreSwing &&
                ctrlIn.w_knee_filtfast > ctrlParsGlobal.w_Hysteresis &&
                ctrlIn.M_autocalib > ctrlParsFSM.M_MinPreSwing &&
                timer_stance > ctrlParsFSM.t_MinStand &&
                ctrlIn.p_thigh < ctrlParsFSM.pt_MinPreSwing;
    return ret_code;
}
static void enf_pre_swing(void ) {
    timer_state = 0.0f;
    ctrlOut.timer_cond_lock = 0.0f;
    ctrlOut.state = PRE_SWING;
}
static void exf_pre_swing(void ) {
    //
}

StateMachine_GuardFunc gfs_pre_swing[MAX_NUMBER_TASKS] = {
    (StateMachine_GuardFunc)gf_pre_swing,
    (StateMachine_GuardFunc)gf_pre_swing,
    (StateMachine_GuardFunc)gf_pre_swing,
    (StateMachine_GuardFunc)gf_pre_swing,
    (StateMachine_GuardFunc)gf_pre_swing,
    (StateMachine_GuardFunc)gf_pre_swing
};
StateMachine_EntryFunc enfs_pre_swing[MAX_NUMBER_TASKS] = {
    (StateMachine_EntryFunc)enf_pre_swing,
    (StateMachine_EntryFunc)enf_pre_swing,
    (StateMachine_EntryFunc)enf_pre_swing,
    (StateMachine_EntryFunc)enf_pre_swing,
    (StateMachine_EntryFunc)enf_pre_swing,
    (StateMachine_EntryFunc)enf_pre_swing
};
StateMachine_StateFunc sfs_pre_swing[MAX_NUMBER_TASKS] = {
    (StateMachine_StateFunc)sf_pre_swing,
    (StateMachine_StateFunc)sf_pre_swing,
    (StateMachine_StateFunc)sf_pre_swing,
    (StateMachine_StateFunc)sf_pre_swing,
    (StateMachine_StateFunc)sf_pre_swing,
    (StateMachine_StateFunc)sf_pre_swing
};
StateMachine_ExitFunc  exfs_pre_swing[MAX_NUMBER_TASKS] = {
    (StateMachine_ExitFunc)exf_pre_swing,
    (StateMachine_ExitFunc)exf_pre_swing,
    (StateMachine_ExitFunc)exf_pre_swing,
    (StateMachine_ExitFunc)exf_pre_swing,
    (StateMachine_ExitFunc)exf_pre_swing,
    (StateMachine_ExitFunc)exf_pre_swing
};
const state_t pre_swing = {
    .state_id = PRE_SWING,
    .p_state_function = sfs_pre_swing,
    .p_guard_function = gfs_pre_swing,
    .p_entry_function = enfs_pre_swing,
    .p_exit_function =  exfs_pre_swing
};

// SWING_FLEXION
static void sf_swing_flexion(void ) {
    PRINT("Swing Flexion\n");
    // timers
    timer_swing += TD; // add to swing timer
    timer_state += TD; // add to custom timer
}
static bool gf_swing_flexion(int previous_state ) {
    PRINT("Guard Swing Flexion\n");
    bool ret_code = false;

    switch(previous_state) {
        case STANCE_FLEXION || STANCE_EXTENSION:
        {
            ret_code =  ctrlIn.F_filtmoving < ctrlParsFSM.F_MaxLift &&
                        ctrlIn.p_knee > ctrlParsFSM.p_SwitchExt &&
                        timer_stance > ctrlParsFSM.t_MinStance;
        } break;

        case PRE_SWING:
        {
            ret_code =  ctrlIn.F_filtmoving < ctrlParsFSM.F_MaxLift ||
                        ctrlIn.p_knee > ctrlParsFSM.p_SwitchExt;
        } break;
    }
}
static void enf_swing_flexion(void ) {
    // reset the timers and move to swing
    timer_stance = 0.0f;
    timer_cond_preswing = 0.0f;
    timer_state = 0.0f;
    ctrlOut.timer_cond_lock = 0.0f; // should already be zero, double check
    ctrlOut.state = SWING_FLEXION;
}
static void exf_swing_flexion(void ) {
    //
}
StateMachine_GuardFunc gfs_swing_flexion[MAX_NUMBER_TASKS] = {
    (StateMachine_GuardFunc)gf_swing_flexion,
    (StateMachine_GuardFunc)gf_swing_flexion,
    (StateMachine_GuardFunc)gf_swing_flexion,
    (StateMachine_GuardFunc)gf_swing_flexion,
    (StateMachine_GuardFunc)gf_swing_flexion,
    (StateMachine_GuardFunc)gf_swing_flexion
};
StateMachine_EntryFunc enfs_swing_flexion[MAX_NUMBER_TASKS] = {
    (StateMachine_EntryFunc)enf_swing_flexion,
    (StateMachine_EntryFunc)enf_swing_flexion,
    (StateMachine_EntryFunc)enf_swing_flexion,
    (StateMachine_EntryFunc)enf_swing_flexion,
    (StateMachine_EntryFunc)enf_swing_flexion,
    (StateMachine_EntryFunc)enf_swing_flexion
};
StateMachine_StateFunc sfs_swing_flexion[MAX_NUMBER_TASKS] = {
    (StateMachine_StateFunc)sf_swing_flexion,
    (StateMachine_StateFunc)sf_swing_flexion,
    (StateMachine_StateFunc)sf_swing_flexion,
    (StateMachine_StateFunc)sf_swing_flexion,
    (StateMachine_StateFunc)sf_swing_flexion,
    (StateMachine_StateFunc)sf_swing_flexion
};
StateMachine_ExitFunc  exfs_swing_flexion[MAX_NUMBER_TASKS] = {
    (StateMachine_ExitFunc)exf_swing_flexion,
    (StateMachine_ExitFunc)exf_swing_flexion,
    (StateMachine_ExitFunc)exf_swing_flexion,
    (StateMachine_ExitFunc)exf_swing_flexion,
    (StateMachine_ExitFunc)exf_swing_flexion,
    (StateMachine_ExitFunc)exf_swing_flexion
};
const state_t swing_flexion = {
    .state_id = SWING_FLEXION,
    .p_state_function = sfs_swing_flexion,
    .p_guard_function = gfs_swing_flexion,
    .p_entry_function = enfs_swing_flexion,
    .p_exit_function =  exfs_swing_flexion
};

// SWING_EXTENSION
static void sf_swing_extension(void ) {
    // timers
    timer_swing += TD; // add to swing timer
    timer_state += TD; // add to custom timer
}
static bool gf_swing_extension(int previous_state ) {
    PRINT("Guard Swing Extension, coming from %d\n", previous_state);
    bool ret_code = false;

    ret_code =  ctrlIn.p_knee < ctrlParsGlobal.p_FlexLimit &&
                ctrlIn.p_knee > ctrlParsFSM.p_MinSwingExt &&
                ctrlIn.w_knee_filtfast < -ctrlParsGlobal.w_Hysteresis &&
                timer_state > ctrlParsFSM.t_MinState;
    
    return ret_code;
}
static void enf_swing_extension(void ) {
    timer_state = 0.0f;
    // auto-calibrate, to be safe only if sensors are not excessively loaded (low absolute M derivative, low M, low F)
    if (fabsf(ctrlIn.Md_filtnormal) < ctrlParsFSM.Md_LimitAutocalib && ctrlIn.M_autocalib < ctrlParsFSM.F_M_LimitAutocalib && ctrlIn.F_autocalib < ctrlParsFSM.F_M_LimitAutocalib)
    {
    devicePars.F_offset = ctrlIn.F;
    devicePars.M_offset = ctrlIn.M;
    }
    ctrlOut.state = SWING_EXTENSION;
}
static void exf_swing_extension(void ) {
    //
}
StateMachine_GuardFunc gfs_swing_extension[MAX_NUMBER_TASKS] = {
    (StateMachine_GuardFunc)gf_swing_extension,
    (StateMachine_GuardFunc)gf_swing_extension,
    (StateMachine_GuardFunc)gf_swing_extension,
    (StateMachine_GuardFunc)gf_swing_extension,
    (StateMachine_GuardFunc)gf_swing_extension,
    (StateMachine_GuardFunc)gf_swing_extension
};
StateMachine_EntryFunc enfs_swing_extension[MAX_NUMBER_TASKS] = {
    (StateMachine_EntryFunc)enf_swing_extension,
    (StateMachine_EntryFunc)enf_swing_extension,
    (StateMachine_EntryFunc)enf_swing_extension,
    (StateMachine_EntryFunc)enf_swing_extension,
    (StateMachine_EntryFunc)enf_swing_extension,
    (StateMachine_EntryFunc)enf_swing_extension
};
StateMachine_StateFunc sfs_swing_extension[MAX_NUMBER_TASKS] = {
    (StateMachine_StateFunc)sf_swing_extension,
    (StateMachine_StateFunc)sf_swing_extension,
    (StateMachine_StateFunc)sf_swing_extension,
    (StateMachine_StateFunc)sf_swing_extension,
    (StateMachine_StateFunc)sf_swing_extension,
    (StateMachine_StateFunc)sf_swing_extension
};
StateMachine_ExitFunc  exfs_swing_extension[MAX_NUMBER_TASKS] = {
    (StateMachine_ExitFunc)exf_swing_extension,
    (StateMachine_ExitFunc)exf_swing_extension,
    (StateMachine_ExitFunc)exf_swing_extension,
    (StateMachine_ExitFunc)exf_swing_extension,
    (StateMachine_ExitFunc)exf_swing_extension,
    (StateMachine_ExitFunc)exf_swing_extension
};
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

int init_knee_fsm ( SystemState initial_state, TaskType initial_task) {
    if (&KneeStateMachine != NULL) {
        if (init_state_machine(&KneeStateMachine, (state_t*)&gait_states[initial_state], allowed_transistion_table, gait_states, MAX_NUMBER_STATES, initial_task) == SM_OK) {
            PRINT("Knee fsm correctly created! \n");
            run_fsm();
            return SM_OK;
        } else {
            PRINT("Error in creating knee fsm");
            return SM_NOK;
        }
    } else {
        PRINT("StateMachine pointer is null");
        return SM_NOK;
    }
    
    
}

int reset_state(SystemState state) {
    (&KneeStateMachine)->current_state = (state_t*)&gait_states[state];
    (&KneeStateMachine)->current_state->p_entry_function[get_current_task(&KneeStateMachine)]();
}
int change_task (TaskType task) {
    change_task_state_machine(&KneeStateMachine, task);
    return reset_state(initial_state_task[task]);
}

int run_fsm () {
    printf("Current task: %s... state: %s \n", task_names[get_current_task(&KneeStateMachine)], state_names[get_current_state(&KneeStateMachine)]);
    printf("%f  %f  %f  %f\n", timer_state, timer_stance, timer_swing, timer_cond_preswing);
    return run_state_machine(&KneeStateMachine);

}