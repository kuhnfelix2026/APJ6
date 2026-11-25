/*
 * APJ6Control.h
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "APJ6Control".
 *
 * Model version              : 2.22
 * Simulink Coder version : 25.2 (R2025b) 28-Jul-2025
 * C source code generated on : Wed Nov 25 16:49:19 2026
 *
 * Target selection: ert.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: ARM Compatible->ARM Cortex
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef APJ6Control_h_
#define APJ6Control_h_
#ifndef APJ6Control_COMMON_INCLUDES_
#define APJ6Control_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* APJ6Control_COMMON_INCLUDES_ */

#include "APJ6Control_types.h"
#include "rt_nonfinite.h"
#include "rtGetNaN.h"
#include <stddef.h>
#include <string.h>

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
  real32_T CCaller;                    /* '<Root>/C Caller' */
  real32_T DiscreteFIRFilter;          /* '<Root>/Discrete FIR Filter' */
  int32_T CCallerinput_o8;             /* '<Root>/C Caller input' */
} B_APJ6Control_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T count;                        /* '<Root>/MATLAB Function1' */
  real32_T DiscreteFIRFilter_states;   /* '<Root>/Discrete FIR Filter' */
  real32_T Integrator_DSTATE;          /* '<S43>/Integrator' */
  real32_T Filter_DSTATE;              /* '<S38>/Filter' */
  real32_T PrevY;                      /* '<Root>/Rate Limiter1' */
  real32_T d1;                         /* '<Root>/MATLAB Function1' */
  real32_T d2;                         /* '<Root>/MATLAB Function1' */
  real32_T dprev;                      /* '<Root>/MATLAB Function1' */
  real32_T rel_f;                      /* '<Root>/MATLAB Function1' */
  int8_T Integrator_PrevResetState;    /* '<S43>/Integrator' */
  int8_T Filter_PrevResetState;        /* '<S38>/Filter' */
} DW_APJ6Control_T;

/* Parameters (default storage) */
struct P_APJ6Control_T_ {
  int32_T CompareToConstant1_const;  /* Mask Parameter: CompareToConstant1_const
                                      * Referenced by: '<S2>/Constant'
                                      */
  real32_T DiscretePIDController_D;   /* Mask Parameter: DiscretePIDController_D
                                       * Referenced by: '<S36>/Derivative Gain'
                                       */
  real32_T DiscretePIDController_I;   /* Mask Parameter: DiscretePIDController_I
                                       * Referenced by: '<S40>/Integral Gain'
                                       */
  real32_T DiscretePIDController_InitialCo;
                              /* Mask Parameter: DiscretePIDController_InitialCo
                               * Referenced by: '<S38>/Filter'
                               */
  real32_T DiscretePIDController_Initial_k;
                              /* Mask Parameter: DiscretePIDController_Initial_k
                               * Referenced by: '<S43>/Integrator'
                               */
  real32_T DiscretePIDController_LowerSatu;
                              /* Mask Parameter: DiscretePIDController_LowerSatu
                               * Referenced by:
                               *   '<S50>/Saturation'
                               *   '<S35>/DeadZone'
                               */
  real32_T DiscretePIDController_N;   /* Mask Parameter: DiscretePIDController_N
                                       * Referenced by: '<S46>/Filter Coefficient'
                                       */
  real32_T DiscretePIDController_P;   /* Mask Parameter: DiscretePIDController_P
                                       * Referenced by: '<S48>/Proportional Gain'
                                       */
  real32_T DiscretePIDController_UpperSatu;
                              /* Mask Parameter: DiscretePIDController_UpperSatu
                               * Referenced by:
                               *   '<S50>/Saturation'
                               *   '<S35>/DeadZone'
                               */
  real32_T CompareToConstant_const;   /* Mask Parameter: CompareToConstant_const
                                       * Referenced by: '<S1>/Constant'
                                       */
  real_T RateLimiter1_RisingLim;       /* Expression: 1500
                                        * Referenced by: '<Root>/Rate Limiter1'
                                        */
  real_T RateLimiter1_FallingLim;      /* Expression: -5000
                                        * Referenced by: '<Root>/Rate Limiter1'
                                        */
  char_T StringConstant_String[256];   /* Expression: "testcar"
                                        * Referenced by: '<S4>/String Constant'
                                        */
  char_T StringConstant1_String[256];  /* Expression: "12345678"
                                        * Referenced by: '<S4>/String Constant1'
                                        */
  real32_T Constant1_Value;            /* Computed Parameter: Constant1_Value
                                        * Referenced by: '<S33>/Constant1'
                                        */
  real32_T DiscreteFIRFilter_InitialStates;
                          /* Computed Parameter: DiscreteFIRFilter_InitialStates
                           * Referenced by: '<Root>/Discrete FIR Filter'
                           */
  real32_T DiscreteFIRFilter_Coefficients[2];
                           /* Computed Parameter: DiscreteFIRFilter_Coefficients
                            * Referenced by: '<Root>/Discrete FIR Filter'
                            */
  real32_T RateLimiter1_IC;            /* Computed Parameter: RateLimiter1_IC
                                        * Referenced by: '<Root>/Rate Limiter1'
                                        */
  real32_T Integrator_gainval;         /* Computed Parameter: Integrator_gainval
                                        * Referenced by: '<S43>/Integrator'
                                        */
  real32_T Filter_gainval;             /* Computed Parameter: Filter_gainval
                                        * Referenced by: '<S38>/Filter'
                                        */
  real32_T Clamping_zero_Value;       /* Computed Parameter: Clamping_zero_Value
                                       * Referenced by: '<S33>/Clamping_zero'
                                       */
  int8_T Constant_Value;               /* Computed Parameter: Constant_Value
                                        * Referenced by: '<S33>/Constant'
                                        */
  int8_T Constant2_Value;              /* Computed Parameter: Constant2_Value
                                        * Referenced by: '<S33>/Constant2'
                                        */
  int8_T Constant3_Value;              /* Computed Parameter: Constant3_Value
                                        * Referenced by: '<S33>/Constant3'
                                        */
  int8_T Constant4_Value;              /* Computed Parameter: Constant4_Value
                                        * Referenced by: '<S33>/Constant4'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_APJ6Control_T {
  const char_T *errorStatus;
};

/* Block parameters (default storage) */
extern P_APJ6Control_T APJ6Control_P;

/* Block signals (default storage) */
extern B_APJ6Control_T APJ6Control_B;

/* Block states (default storage) */
extern DW_APJ6Control_T APJ6Control_DW;

/* Model entry point functions */
extern void APJ6Control_initialize(void);
extern void APJ6Control_step(void);
extern void APJ6Control_terminate(void);

/* Real-time Model object */
extern RT_MODEL_APJ6Control_T *const APJ6Control_M;
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
 * '<Root>' : 'APJ6Control'
 * '<S1>'   : 'APJ6Control/Compare To Constant'
 * '<S2>'   : 'APJ6Control/Compare To Constant1'
 * '<S3>'   : 'APJ6Control/Discrete PID Controller'
 * '<S4>'   : 'APJ6Control/Initialize Function'
 * '<S5>'   : 'APJ6Control/MATLAB Function1'
 * '<S6>'   : 'APJ6Control/Terminate Function'
 * '<S7>'   : 'APJ6Control/Discrete PID Controller/Anti-windup'
 * '<S8>'   : 'APJ6Control/Discrete PID Controller/D Gain'
 * '<S9>'   : 'APJ6Control/Discrete PID Controller/External Derivative'
 * '<S10>'  : 'APJ6Control/Discrete PID Controller/Filter'
 * '<S11>'  : 'APJ6Control/Discrete PID Controller/Filter ICs'
 * '<S12>'  : 'APJ6Control/Discrete PID Controller/I Gain'
 * '<S13>'  : 'APJ6Control/Discrete PID Controller/Ideal P Gain'
 * '<S14>'  : 'APJ6Control/Discrete PID Controller/Ideal P Gain Fdbk'
 * '<S15>'  : 'APJ6Control/Discrete PID Controller/Integrator'
 * '<S16>'  : 'APJ6Control/Discrete PID Controller/Integrator ICs'
 * '<S17>'  : 'APJ6Control/Discrete PID Controller/N Copy'
 * '<S18>'  : 'APJ6Control/Discrete PID Controller/N Gain'
 * '<S19>'  : 'APJ6Control/Discrete PID Controller/P Copy'
 * '<S20>'  : 'APJ6Control/Discrete PID Controller/Parallel P Gain'
 * '<S21>'  : 'APJ6Control/Discrete PID Controller/Reset Signal'
 * '<S22>'  : 'APJ6Control/Discrete PID Controller/Saturation'
 * '<S23>'  : 'APJ6Control/Discrete PID Controller/Saturation Fdbk'
 * '<S24>'  : 'APJ6Control/Discrete PID Controller/Sum'
 * '<S25>'  : 'APJ6Control/Discrete PID Controller/Sum Fdbk'
 * '<S26>'  : 'APJ6Control/Discrete PID Controller/Tracking Mode'
 * '<S27>'  : 'APJ6Control/Discrete PID Controller/Tracking Mode Sum'
 * '<S28>'  : 'APJ6Control/Discrete PID Controller/Tsamp - Integral'
 * '<S29>'  : 'APJ6Control/Discrete PID Controller/Tsamp - Ngain'
 * '<S30>'  : 'APJ6Control/Discrete PID Controller/postSat Signal'
 * '<S31>'  : 'APJ6Control/Discrete PID Controller/preInt Signal'
 * '<S32>'  : 'APJ6Control/Discrete PID Controller/preSat Signal'
 * '<S33>'  : 'APJ6Control/Discrete PID Controller/Anti-windup/Disc. Clamping Parallel'
 * '<S34>'  : 'APJ6Control/Discrete PID Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone'
 * '<S35>'  : 'APJ6Control/Discrete PID Controller/Anti-windup/Disc. Clamping Parallel/Dead Zone/Enabled'
 * '<S36>'  : 'APJ6Control/Discrete PID Controller/D Gain/Internal Parameters'
 * '<S37>'  : 'APJ6Control/Discrete PID Controller/External Derivative/Error'
 * '<S38>'  : 'APJ6Control/Discrete PID Controller/Filter/Disc. Forward Euler Filter'
 * '<S39>'  : 'APJ6Control/Discrete PID Controller/Filter ICs/Internal IC - Filter'
 * '<S40>'  : 'APJ6Control/Discrete PID Controller/I Gain/Internal Parameters'
 * '<S41>'  : 'APJ6Control/Discrete PID Controller/Ideal P Gain/Passthrough'
 * '<S42>'  : 'APJ6Control/Discrete PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S43>'  : 'APJ6Control/Discrete PID Controller/Integrator/Discrete'
 * '<S44>'  : 'APJ6Control/Discrete PID Controller/Integrator ICs/Internal IC'
 * '<S45>'  : 'APJ6Control/Discrete PID Controller/N Copy/Disabled'
 * '<S46>'  : 'APJ6Control/Discrete PID Controller/N Gain/Internal Parameters'
 * '<S47>'  : 'APJ6Control/Discrete PID Controller/P Copy/Disabled'
 * '<S48>'  : 'APJ6Control/Discrete PID Controller/Parallel P Gain/Internal Parameters'
 * '<S49>'  : 'APJ6Control/Discrete PID Controller/Reset Signal/External Reset'
 * '<S50>'  : 'APJ6Control/Discrete PID Controller/Saturation/Enabled'
 * '<S51>'  : 'APJ6Control/Discrete PID Controller/Saturation Fdbk/Disabled'
 * '<S52>'  : 'APJ6Control/Discrete PID Controller/Sum/Sum_PID'
 * '<S53>'  : 'APJ6Control/Discrete PID Controller/Sum Fdbk/Disabled'
 * '<S54>'  : 'APJ6Control/Discrete PID Controller/Tracking Mode/Disabled'
 * '<S55>'  : 'APJ6Control/Discrete PID Controller/Tracking Mode Sum/Passthrough'
 * '<S56>'  : 'APJ6Control/Discrete PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S57>'  : 'APJ6Control/Discrete PID Controller/Tsamp - Ngain/Passthrough'
 * '<S58>'  : 'APJ6Control/Discrete PID Controller/postSat Signal/Forward_Path'
 * '<S59>'  : 'APJ6Control/Discrete PID Controller/preInt Signal/Internal PreInt'
 * '<S60>'  : 'APJ6Control/Discrete PID Controller/preSat Signal/Forward_Path'
 */
#endif                                 /* APJ6Control_h_ */
