/*
 * APJ6Control_data.c
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

#include "APJ6Control.h"

/* Block parameters (default storage) */
P_APJ6Control_T APJ6Control_P = {
  /* Mask Parameter: CompareToConstant1_const
   * Referenced by: '<S2>/Constant'
   */
  0,

  /* Mask Parameter: DiscretePIDController_D
   * Referenced by: '<S36>/Derivative Gain'
   */
  0.0F,

  /* Mask Parameter: DiscretePIDController_I
   * Referenced by: '<S40>/Integral Gain'
   */
  0.25F,

  /* Mask Parameter: DiscretePIDController_InitialCo
   * Referenced by: '<S38>/Filter'
   */
  0.0F,

  /* Mask Parameter: DiscretePIDController_Initial_k
   * Referenced by: '<S43>/Integrator'
   */
  0.0F,

  /* Mask Parameter: DiscretePIDController_LowerSatu
   * Referenced by:
   *   '<S50>/Saturation'
   *   '<S35>/DeadZone'
   */
  0.0F,

  /* Mask Parameter: DiscretePIDController_N
   * Referenced by: '<S46>/Filter Coefficient'
   */
  100.0F,

  /* Mask Parameter: DiscretePIDController_P
   * Referenced by: '<S48>/Proportional Gain'
   */
  0.15F,

  /* Mask Parameter: DiscretePIDController_UpperSatu
   * Referenced by:
   *   '<S50>/Saturation'
   *   '<S35>/DeadZone'
   */
  200.0F,

  /* Mask Parameter: CompareToConstant_const
   * Referenced by: '<S1>/Constant'
   */
  0.0F,

  /* Expression: 1500
   * Referenced by: '<Root>/Rate Limiter1'
   */
  1500.0,

  /* Expression: -5000
   * Referenced by: '<Root>/Rate Limiter1'
   */
  -5000.0,

  /* Expression: "testcar"
   * Referenced by: '<S4>/String Constant'
   */
  "testcar",

  /* Expression: "12345678"
   * Referenced by: '<S4>/String Constant1'
   */
  "12345678",

  /* Computed Parameter: Constant1_Value
   * Referenced by: '<S33>/Constant1'
   */
  0.0F,

  /* Computed Parameter: DiscreteFIRFilter_InitialStates
   * Referenced by: '<Root>/Discrete FIR Filter'
   */
  0.0F,

  /* Computed Parameter: DiscreteFIRFilter_Coefficients
   * Referenced by: '<Root>/Discrete FIR Filter'
   */
  { 0.5F, 0.5F },

  /* Computed Parameter: RateLimiter1_IC
   * Referenced by: '<Root>/Rate Limiter1'
   */
  0.0F,

  /* Computed Parameter: Integrator_gainval
   * Referenced by: '<S43>/Integrator'
   */
  0.05F,

  /* Computed Parameter: Filter_gainval
   * Referenced by: '<S38>/Filter'
   */
  0.05F,

  /* Computed Parameter: Clamping_zero_Value
   * Referenced by: '<S33>/Clamping_zero'
   */
  0.0F,

  /* Computed Parameter: Constant_Value
   * Referenced by: '<S33>/Constant'
   */
  1,

  /* Computed Parameter: Constant2_Value
   * Referenced by: '<S33>/Constant2'
   */
  -1,

  /* Computed Parameter: Constant3_Value
   * Referenced by: '<S33>/Constant3'
   */
  1,

  /* Computed Parameter: Constant4_Value
   * Referenced by: '<S33>/Constant4'
   */
  -1
};
