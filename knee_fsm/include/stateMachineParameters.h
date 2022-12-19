#ifndef STATE_MACHINE_PARAMETERS_H
#define STATE_MACHINE_PARAMETERS_H

#include <stdint.h>
#include <stdbool.h>
#include "utils/filters.h"

#define PATIENT_MASS_MIN 60.0f  // [kg]
#define PATIENT_MASS_MAX 120.0f // [kg]
#define DAMPING_MIN 0.0f        // min damping of valves
#define DAMPING_MAX 1.0f        // max damping of valves
#define X_0_MIN 0.0f            // min valve set-up alignment parameter
#define X_0_MAX 1.0f            // max valve set-up alignment parameter


typedef enum
{
    STANCE_FLEXION,
    STANCE_EXTENSION,
    PRE_SWING,
    SWING_FLEXION,
    SWING_EXTENSION,
    MAX_NUMBER_STATES
} SystemState;

typedef enum
{
    WALK = 0,
    SIT,
    STAIRS_UP,
    STAIRS_DOWN,
    SLOPE_UP,
    SLOPE_DOWN,
    MAX_NUMBER_TASKS
} TaskType;

//*****************************************************************************
// Structs declaration
//*****************************************************************************

// general purposes variable for testing
typedef struct 
{
    float var_0;
    float var_1;
    float var_2;
    float var_3;
    float var_4;
    float var_5;
    float var_6;
    float var_7;
    float var_8;
    float var_9;
} GeneralPurpose;

// Variables configured from android app
typedef struct 
{
    float var_0;
    float var_1;
    float var_2;
    float var_3;
    float var_4;
    float var_5;
    float var_6;
    float var_7;
    float var_8;
    float var_9;
} AppVariables;

typedef struct
{
    float mass;
    float length_thigh;
    float length_shank;
    float height_foot;
    float length_foot;
} UserParameters;

// Input data to the FSM:
typedef struct
{
    // Board data processed and filtered: 
    float p_knee;           // potentiometer(knee) position
    float w_knee;               // potentiometer(knee) velocity raw
    float w_knee_filtfast;      // low pass filtered signal of knee velocity with fast f (mainly used for FSM)
    float w_knee_filtnormal;    // low pass filtered signal of knee velocity with medium f (mainly used for middle layer)
    float w_knee_filtslow;      // low pass filtered signal of knee velocity with slow f (mainly used for middle layer)
    float p_shank;          // shank position
    float w_shank;
    float p_thigh;          // thigh position
    float w_thigh;          // thigh velocity
    float w_thigh_filtslow;     // low-pass filtered thigh velocity with slow c/o frequency
    float p_motor;          // motor position in rad
    float w_motor;      // motor velocity in rad   
    float w_motor_filtfast;     // low pass filtered signal of motor velocity with fast f (used for middle layer)
    float w_motor_filtnormal;   // low pass filtered signal of motor velocity with medium f (used for middle layer)
    float w_motor_filtslow;     // low pass filtered signal of motor velocity with slow f (used for middle layer)
    float F;                // tube axial F raw
    float F_filtmoving;           // tube axial F (=F - Fz_old)
    float F_autocalib;      // tube axial F auto-calibrated (= F - F_offset)
    float M;                // tube moment 
    float Md;          // tube moment derivative
    float Md_filtnormal;     // tube moment derivative filtered
    float M_autocalib;      // tube moment auto-calibrated value (= moment - M_offset)
} ControlInputs;

// Output data from the controller:
typedef struct
{
    TaskType task;            // bool for task
    SystemState state; // always start in state 1 (StandFlexion)
    uint8_t actuation_flag;         // must be 1 initially because we start seated, activating the motor. This is not a parameter, so it should go somewhere else.
    float timer_cond_lock;
    float vel_ambulation;              // ambulation velocity [s]
} ControlOutputs;

typedef struct{
    // calibration data
    float xvu_closure;
    float xvo_closure;
    float p_motor_offset;
    float p_knee_offset;        
    float M_offset;           // moment offset [s] -> controller, manager, transitions (it's the offset for the autocalibration)
    float F_offset;          // F offset [s]
} DeviceParameters;

// Parameters used in the State Machine as well as in the Middlelayer Control:
typedef struct
{
    // new parameters
    float p_Hysteresis;
    float p_ExtLimit; // was mlParameters.pk_extlim
    float p_FlexLimit;   // was mlParameters.pk_flxlim 3,4,5 & fsmParameters.p_FlxMax

    float p_FlexLimitDescent; // TO BE USED by slope down
    float p_FlexResist; // was p_FlexResist    
    float p_Activate; // was p_StandUp

    // angular velocity threshold FSM
    float w_Hysteresis;   // (+inc/-inc) velocity threshold
    float w_Motion;   // (inc) flexing velocity back to stance flex, >=w_Hysteresis

} ControlParametersGlobal;

typedef struct {
    // time thresholds FSM
    float t_MinState;   // time general minimum (conditional) duration for all states and some transitions, except PSw
    float t_MinStanceFlex; // minimum duration in stance before going to StE
    float t_MinStand; // minimum duration in stance before going to PSw
    float t_MinStance; // minimum duration in stance before going to SwF 
    float t_MinCondLock; // minimum conditional duration to activate stand lock
    float t_MinCondPreSwing;   // threshold time constant for switching to pre-swing under special conditions [t_MinCondPreSwing>t_2, t_MinCondPreSwing>timer_stance]
    float t_Fallback; // threshold time to ease conditions to fall back (reset=Rst) to StF (from PSw and SwF)

    // angle thresholds FSM
    float p_SwitchExt;   // (dec) switch to StE
    float p_MinSwingExt; // (inc) switch to SwE (>p_SwitchExt)
    float pt_MinPreSwing;
    float pt_MinSeated; // (inc) switch to sit while already seated
    float p_Fallback; // (dec) lazy switch to StF (from SwF)

    // moment thresholds FSM
    float M_MinPreSwing;   // (inc) extension moment: ca. 20% of maximum during extension *p.m
    float M_Fallback; // (dec) moment for recovery from PSw *p.m 
    float M_Hysteresis; // (inc) positive moment for detecting toe load to stand up (>0, <M_Fallback<M_MinPreSwing)

    // F thresholds FSM
    float F_Hysteresis;
    float F_SwitchContact;  // (inc) relative F transition from swing to stance
    float F_MaxLift;  // (dec) relative F transition between stance to swing
    float F_Fallback; // (inc) F transition for recovery from PSw *p.m
    float F_SwitchSupport;
    float F_M_LimitAutocalib;
    float Md_LimitAutocalib;
} ControlParametersStateTransitions;

typedef struct {
	float M_MinToe;
	float M_MaxHeel;
	float Md_MinToe;
	float Md_MinHeel;
	float t_Limit;
	bool enableToeSwitch;
	bool enableHeelSwitch;
	bool enableAutoSwitchStairsUp; // TO BE USED
    bool enableAutoSwitchStairsDown; // TO BE USED
	bool enableAutoSwitchSlopes; // TO BE USED
} ControlParametersTaskSwitch;

typedef struct {
    bool enableStanceLock; // flag to enable or disable automatic locking feature during stance
    
    // default damping (damping when not in flexion or extension limit)
    float xv_Max; // uni-directional valve: not state-dependent

    float xv_Stance;     // was xvo_base[0] & xvo_base[1]
    float xv_SwingFlex; // was xvo_base[2] & xvo_base[3]
    float xv_SwingExt;  // was xvo_base[4]

    float xv_StanceDescent;    // TO BE USED by ramp(/stairs?) down
    float xv_SwingFlexDescent; // TO BE USED by ramp(/stairs?) down
    float xv_SwingExtDescent;  // TO BE USED by ramp(/stairs?) down

    // special damping
    float xv_SwingExtStairs; // swing extension resistance

    float xv_Endstop;
    float ramp_Endstop;
    float exponent_Endstop;

    // actuation: sitting only
    float xv_Flex;
    float exponent_Flex;

} ControlParametersValves;

typedef struct
{
    // enabling flags
    bool enableExtSupport; // used to enable support in extension during stair and slope climbing (typically during stance)
    bool enableFlexSupport; // used to enable support in flexion during stair and slope climbing (typically during swing)
    
    // actuation: sitting & stairs (common)
    float T_Max; // [Nm] maximum absolute motor torque
    float p_TargetStairs; // maximum knee angle at which actuation is still possible while climbing stairs
    float p_TargetSit; // maximum knee angle at which actuation is still possible while standing up
    float k_Target; // proportional gain torque to knee angle
    float c_Target;
    float ps_Off;  // maximum shank angle to exert torque
    float ps_On;   // maximum shank angle at which torque is not saturated

    // actuation: clutch-related ((dis)engagement) (common)
    float T_MaxDisengaged;   //[Nm] max abs motor torque when not engaged (trailing/to rest target)
    float prel_Engaged;    // [rad] maximum relative angle to start pushing clutch with 100% commanded torque
    float prel_Disengaged;     // [rad] fuzzy range relative angle for trailing vs pushing

    float prel_Trail;   // [rad] target trailing relative angle
    float prel_Stalk;  // [rad] target escape relative angle (when flicking for stair ascent)
    float krel_Disengaged; // proportional gain torque to relative angle for trailing purposes
    float crel_Disengaged; // damping torque to relative angle for trailing purposes
    float krel_Engage;
    float crel_Engage;

    float pm_TargetIdle;     //[rad] motor rest target angle
    float pm_RangeIdle; //[rad] acceptable error from rest target
    float t_WaitIdle;     //[s] reset time for motor angle
    float t_Disable;

    float timer_motor_active; //[s] timer for angle (not a parameter! shouldn't be here!)

} ControlParametersActuator;

extern FiltValues filtValues;
extern UserParameters userPars;
extern DeviceParameters devicePars;
extern ControlInputs  ctrlIn;
extern ControlOutputs ctrlOut;
extern ControlParametersStateTransitions ctrlParsFSM;
extern ControlParametersTaskSwitch ctrlParsTask;
extern ControlParametersGlobal ctrlParsGlobal;
extern ControlParametersActuator ctrlParsAct;
extern ControlParametersValves ctrlParsValves;


extern GeneralPurpose generalPurpose;
extern AppVariables appVariables;

#endif //STATE_MACHINE_PARAMETERS_H
