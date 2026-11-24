/*
 * CarreraUdpTest.h
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "CarreraUdpTest".
 *
 * Model version              : 2.2
 * Simulink Coder version : 25.2 (R2025b) 28-Jul-2025
 * C source code generated on : Tue Nov 24 15:52:29 2026
 *
 * Target selection: ert.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef CarreraUdpTest_h_
#define CarreraUdpTest_h_
#ifndef CarreraUdpTest_COMMON_INCLUDES_
#define CarreraUdpTest_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* CarreraUdpTest_COMMON_INCLUDES_ */

#include "CarreraUdpTest_types.h"
#include <stddef.h>
#include <string.h>
#include "rt_nonfinite.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* user code (top of header file) */
#include "APJ6_interface.h"

/* Block signals (default storage) */
typedef struct {
  char_T StringConstant[256];          /* '<S4>/String Constant' */
  char_T StringConstant1[256];         /* '<S4>/String Constant1' */
  real32_T CCallerinput_o1;            /* '<Root>/C Caller input' */
  real32_T CCallerinput_o2;            /* '<Root>/C Caller input' */
  real32_T CCallerinput_o3;            /* '<Root>/C Caller input' */
  real32_T CCallerinput_o4;            /* '<Root>/C Caller input' */
  real32_T CCallerinput_o5;            /* '<Root>/C Caller input' */
  real32_T CCallerinput_o6;            /* '<Root>/C Caller input' */
  real32_T CCallerinput_o7;            /* '<Root>/C Caller input' */
  int32_T CCallerinput_o8;             /* '<Root>/C Caller input' */
} B_CarreraUdpTest_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real32_T Integrator_DSTATE;          /* '<S42>/Integrator' */
  real32_T Filter_DSTATE;              /* '<S37>/Filter' */
  int8_T Integrator_PrevResetState;    /* '<S42>/Integrator' */
  int8_T Filter_PrevResetState;        /* '<S37>/Filter' */
} DW_CarreraUdpTest_T;

/* Parameters (default storage) */
struct P_CarreraUdpTest_T_ {
  int32_T CompareToConstant1_const;  /* Mask Parameter: CompareToConstant1_const
                                      * Referenced by: '<S2>/Constant'
                                      */
  real32_T DiscretePIDController_D;   /* Mask Parameter: DiscretePIDController_D
                                       * Referenced by: '<S35>/Derivative Gain'
                                       */
  real32_T DiscretePIDController_I;   /* Mask Parameter: DiscretePIDController_I
                                       * Referenced by: '<S39>/Integral Gain'
                                       */
  real32_T DiscretePIDController_InitialCo;
                              /* Mask Parameter: DiscretePIDController_InitialCo
                               * Referenced by: '<S37>/Filter'
                               */
  real32_T DiscretePIDController_Initial_k;
                              /* Mask Parameter: DiscretePIDController_Initial_k
                               * Referenced by: '<S42>/Integrator'
                               */
  real32_T DiscretePIDController_LowerSatu;
                              /* Mask Parameter: DiscretePIDController_LowerSatu
                               * Referenced by:
                               *   '<S49>/Saturation'
                               *   '<S34>/DeadZone'
                               */
  real32_T DiscretePIDController_N;   /* Mask Parameter: DiscretePIDController_N
                                       * Referenced by: '<S45>/Filter Coefficient'
                                       */
  real32_T DiscretePIDController_P;   /* Mask Parameter: DiscretePIDController_P
                                       * Referenced by: '<S47>/Proportional Gain'
                                       */
  real32_T DiscretePIDController_UpperSatu;
                              /* Mask Parameter: DiscretePIDController_UpperSatu
                               * Referenced by:
                               *   '<S49>/Saturation'
                               *   '<S34>/DeadZone'
                               */
  real32_T CompareToConstant_const;   /* Mask Parameter: CompareToConstant_const
                                       * Referenced by: '<S1>/Constant'
                                       */
  char_T StringConstant_String[256];   /* Expression: "testcar"
                                        * Referenced by: '<S4>/String Constant'
                                        */
  char_T StringConstant1_String[256];  /* Expression: "12345678"
                                        * Referenced by: '<S4>/String Constant1'
                                        */
  real32_T Constant1_Value;            /* Computed Parameter: Constant1_Value
                                        * Referenced by: '<S32>/Constant1'
                                        */
  real32_T Gain_Gain;                  /* Computed Parameter: Gain_Gain
                                        * Referenced by: '<Root>/Gain'
                                        */
  real32_T Integrator_gainval;         /* Computed Parameter: Integrator_gainval
                                        * Referenced by: '<S42>/Integrator'
                                        */
  real32_T Filter_gainval;             /* Computed Parameter: Filter_gainval
                                        * Referenced by: '<S37>/Filter'
                                        */
  real32_T Clamping_zero_Value;       /* Computed Parameter: Clamping_zero_Value
                                       * Referenced by: '<S32>/Clamping_zero'
                                       */
  int8_T Constant_Value;               /* Computed Parameter: Constant_Value
                                        * Referenced by: '<S32>/Constant'
                                        */
  int8_T Constant2_Value;              /* Computed Parameter: Constant2_Value
                                        * Referenced by: '<S32>/Constant2'
                                        */
  int8_T Constant3_Value;              /* Computed Parameter: Constant3_Value
                                        * Referenced by: '<S32>/Constant3'
                                        */
  int8_T Constant4_Value;              /* Computed Parameter: Constant4_Value
                                        * Referenced by: '<S32>/Constant4'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_CarreraUdpTest_T {
  const char_T *errorStatus;
};

/* Block parameters (default storage) */
extern P_CarreraUdpTest_T CarreraUdpTest_P;

/* Block signals (default storage) */
extern B_CarreraUdpTest_T CarreraUdpTest_B;

/* Block states (default storage) */
extern DW_CarreraUdpTest_T CarreraUdpTest_DW;

/* Model entry point functions */
extern void CarreraUdpTest_initialize(void);
extern void CarreraUdpTest_step(void);
extern void CarreraUdpTest_terminate(void);

/* Real-time Model object */
extern RT_MODEL_CarreraUdpTest_T *const CarreraUdpTest_M;
extern volatile boolean_T stopRequested;
extern volatile boolean_T runModel;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'CarreraUdpTest'
 * '<S1>'   : 'CarreraUdpTest/Compare To Constant'
 * '<S2>'   : 'CarreraUdpTest/Compare To Constant1'
 * '<S3>'   : 'CarreraUdpTest/Discrete PID Controller'
 * '<S4>'   : 'CarreraUdpTest/Initialize Function'
 * '<S5>'   : 'CarreraUdpTest/Terminate Function'
 * '<S6>'   : 'CarreraUdpTest/Discrete PID Controller/Anti-windup'
 * '<S7>'   : 'CarreraUdpTest/Discrete PID Controller/D Gain'
 * '<S8>'   : 'CarreraUdpTest/Discrete PID Controller/External Derivative'
 * '<S9>'   : 'CarreraUdpTest/Discrete PID Controller/Filter'
 * '<S10>'  : 'CarreraUdpTest/Discrete PID Controller/Filter ICs'
 * '<S11>'  : 'CarreraUdpTest/Discrete PID Controller/I Gain'
 * '<S12>'  : 'CarreraUdpTest/Discrete PID Controller/Ideal P Gain'
 * '<S13>'  : 'CarreraUdpTest/Discrete PID Controller/Ideal P Gain Fdbk'
 * '<S14>'  : 'CarreraUdpTest/Discrete PID Controller/Integrator'
 * '<S15>'  : 'CarreraUdpTest/Discrete PID Controller/Integrator ICs'
 * '<S16>'  : 'CarreraUdpTest/Discrete PID Controller/N Copy'
 * '<S17>'  : 'CarreraUdpTest/Discrete PID Controller/N Gain'
 * '<S18>'  : 'CarreraUdpTest/Discrete PID Controller/P Copy'
 * '<S19>'  : 'CarreraUdpTest/Discrete PID Controller/Parallel P Gain'
 * '<S20>'  : 'CarreraUdpTest/Discrete PID Controller/Reset Signal'
 * '<S21>'  : 'CarreraUdpTest/Discrete PID Controller/Saturation'
 * '<S22>'  : 'CarreraUdpTest/Discrete PID Controller/Saturation Fdbk'
 * '<S23>'  : 'CarreraUdpTest/Discrete PID Controller/Sum'
 * '<S24>'  : 'CarreraUdpTest/Discrete PID Controller/Sum Fdbk'
 * '<S25>'  : 'CarreraUdpTest/Discrete PID Controller/Tracking Mode'
 * '<S26>'  : 'CarreraUdpTest/Discrete PID Controller/Tracking Mode Sum'
 * '<S27>'  : 'CarreraUdpTest/Discrete PID Controller/Tsamp - Integral'
 * '<S28>'  : 'CarreraUdpTest/Discrete PID Controller/Tsamp - Ngain'
 * '<S29>'  : 'CarreraUdpTest/Discrete PID Controller/postSat Signal'
 * '<S30>'  : 'CarreraUdpTest/Discrete PID Controller/preInt Signal'
 * '<S31>'  : 'CarreraUdpTest/Discrete PID Controller/preSat Signal'
 * '<S32>'  : 'CarreraUdpTest/Discrete PID Controller/Anti-windup/Disc. Clamping Parallel'
 * '<S33>'  : 'CarreraUdpTest/Discrete PID Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S34>'  : 'CarreraUdpTest/Discrete PID Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S35>'  : 'CarreraUdpTest/Discrete PID Controller/D Gain/Internal Parameters'
 * '<S36>'  : 'CarreraUdpTest/Discrete PID Controller/External Derivative/Error'
 * '<S37>'  : 'CarreraUdpTest/Discrete PID Controller/Filter/Disc. Forward Euler Filter'
 * '<S38>'  : 'CarreraUdpTest/Discrete PID Controller/Filter ICs/Internal IC - Filter'
 * '<S39>'  : 'CarreraUdpTest/Discrete PID Controller/I Gain/Internal Parameters'
 * '<S40>'  : 'CarreraUdpTest/Discrete PID Controller/Ideal P Gain/Passthrough'
 * '<S41>'  : 'CarreraUdpTest/Discrete PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S42>'  : 'CarreraUdpTest/Discrete PID Controller/Integrator/Discrete'
 * '<S43>'  : 'CarreraUdpTest/Discrete PID Controller/Integrator ICs/Internal IC'
 * '<S44>'  : 'CarreraUdpTest/Discrete PID Controller/N Copy/Disabled'
 * '<S45>'  : 'CarreraUdpTest/Discrete PID Controller/N Gain/Internal Parameters'
 * '<S46>'  : 'CarreraUdpTest/Discrete PID Controller/P Copy/Disabled'
 * '<S47>'  : 'CarreraUdpTest/Discrete PID Controller/Parallel P Gain/Internal Parameters'
 * '<S48>'  : 'CarreraUdpTest/Discrete PID Controller/Reset Signal/External Reset'
 * '<S49>'  : 'CarreraUdpTest/Discrete PID Controller/Saturation/Enabled'
 * '<S50>'  : 'CarreraUdpTest/Discrete PID Controller/Saturation Fdbk/Disabled'
 * '<S51>'  : 'CarreraUdpTest/Discrete PID Controller/Sum/Sum_PID'
 * '<S52>'  : 'CarreraUdpTest/Discrete PID Controller/Sum Fdbk/Disabled'
 * '<S53>'  : 'CarreraUdpTest/Discrete PID Controller/Tracking Mode/Disabled'
 * '<S54>'  : 'CarreraUdpTest/Discrete PID Controller/Tracking Mode Sum/Passthrough'
 * '<S55>'  : 'CarreraUdpTest/Discrete PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S56>'  : 'CarreraUdpTest/Discrete PID Controller/Tsamp - Ngain/Passthrough'
 * '<S57>'  : 'CarreraUdpTest/Discrete PID Controller/postSat Signal/Forward_Path'
 * '<S58>'  : 'CarreraUdpTest/Discrete PID Controller/preInt Signal/Internal PreInt'
 * '<S59>'  : 'CarreraUdpTest/Discrete PID Controller/preSat Signal/Forward_Path'
 */
#endif                                 /* CarreraUdpTest_h_ */
