/*
 * CarreraUdpTest_data.c
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

#include "CarreraUdpTest.h"

/* Block parameters (default storage) */
P_CarreraUdpTest_T CarreraUdpTest_P = {
  /* Mask Parameter: CompareToConstant1_const
   * Referenced by: '<S2>/Constant'
   */
  0,

  /* Mask Parameter: DiscretePIDController_D
   * Referenced by: '<S35>/Derivative Gain'
   */
  0.0F,

  /* Mask Parameter: DiscretePIDController_I
   * Referenced by: '<S39>/Integral Gain'
   */
  0.5F,

  /* Mask Parameter: DiscretePIDController_InitialCo
   * Referenced by: '<S37>/Filter'
   */
  0.0F,

  /* Mask Parameter: DiscretePIDController_Initial_k
   * Referenced by: '<S42>/Integrator'
   */
  0.0F,

  /* Mask Parameter: DiscretePIDController_LowerSatu
   * Referenced by:
   *   '<S49>/Saturation'
   *   '<S34>/DeadZone'
   */
  0.0F,

  /* Mask Parameter: DiscretePIDController_N
   * Referenced by: '<S45>/Filter Coefficient'
   */
  100.0F,

  /* Mask Parameter: DiscretePIDController_P
   * Referenced by: '<S47>/Proportional Gain'
   */
  0.1F,

  /* Mask Parameter: DiscretePIDController_UpperSatu
   * Referenced by:
   *   '<S49>/Saturation'
   *   '<S34>/DeadZone'
   */
  150.0F,

  /* Mask Parameter: CompareToConstant_const
   * Referenced by: '<S1>/Constant'
   */
  0.0F,

  /* Expression: "testcar"
   * Referenced by: '<S4>/String Constant'
   */
  "testcar",

  /* Expression: "12345678"
   * Referenced by: '<S4>/String Constant1'
   */
  "12345678",

  /* Computed Parameter: Constant1_Value
   * Referenced by: '<S32>/Constant1'
   */
  0.0F,

  /* Computed Parameter: Gain_Gain
   * Referenced by: '<Root>/Gain'
   */
  10.0F,

  /* Computed Parameter: Integrator_gainval
   * Referenced by: '<S42>/Integrator'
   */
  0.05F,

  /* Computed Parameter: Filter_gainval
   * Referenced by: '<S37>/Filter'
   */
  0.05F,

  /* Computed Parameter: Clamping_zero_Value
   * Referenced by: '<S32>/Clamping_zero'
   */
  0.0F,

  /* Computed Parameter: Constant_Value
   * Referenced by: '<S32>/Constant'
   */
  1,

  /* Computed Parameter: Constant2_Value
   * Referenced by: '<S32>/Constant2'
   */
  -1,

  /* Computed Parameter: Constant3_Value
   * Referenced by: '<S32>/Constant3'
   */
  1,

  /* Computed Parameter: Constant4_Value
   * Referenced by: '<S32>/Constant4'
   */
  -1
};
