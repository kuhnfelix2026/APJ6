/*
 * CarreraUdpTest.c
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
#include <string.h>
#include <math.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"

/* Block signals (default storage) */
B_CarreraUdpTest_T CarreraUdpTest_B;

/* Block states (default storage) */
DW_CarreraUdpTest_T CarreraUdpTest_DW;

/* Real-time model */
static RT_MODEL_CarreraUdpTest_T CarreraUdpTest_M_;
RT_MODEL_CarreraUdpTest_T *const CarreraUdpTest_M = &CarreraUdpTest_M_;

/* Model step function */
void CarreraUdpTest_step(void)
{
  int32_T rtb_DataTypeConversion;
  real32_T rtb_CCallerbatt;
  real32_T rtb_DeadZone;
  real32_T rtb_Gain;
  real32_T rtb_IntegralGain;
  real32_T rtb_Saturation;
  int8_T tmp;
  int8_T tmp_0;
  boolean_T rtb_Compare;
  boolean_T rtb_Compare_p;

  /* CCaller: '<Root>/C Caller distance' */
  rtb_DataTypeConversion = APJ6_Car_get_distance();

  /* CCaller: '<Root>/C Caller input' */
  APJ6_Car_receive_signals(&CarreraUdpTest_B.CCallerinput_o1,
    &CarreraUdpTest_B.CCallerinput_o2, &CarreraUdpTest_B.CCallerinput_o3,
    &CarreraUdpTest_B.CCallerinput_o4, &CarreraUdpTest_B.CCallerinput_o5,
    &CarreraUdpTest_B.CCallerinput_o6, &CarreraUdpTest_B.CCallerinput_o7,
    &CarreraUdpTest_B.CCallerinput_o8);

  /* CCaller: '<Root>/C Caller' */
  rtb_IntegralGain = APJ6_Car_get_speed();

  /* Gain: '<Root>/Gain' */
  rtb_Gain = CarreraUdpTest_P.Gain_Gain * CarreraUdpTest_B.CCallerinput_o1;

  /* Sum: '<Root>/Sum' */
  rtb_IntegralGain = rtb_Gain - rtb_IntegralGain;

  /* RelationalOperator: '<S1>/Compare' incorporates:
   *  Constant: '<S1>/Constant'
   */
  rtb_Compare = (CarreraUdpTest_B.CCallerinput_o1 <=
                 CarreraUdpTest_P.CompareToConstant_const);

  /* RelationalOperator: '<S2>/Compare' incorporates:
   *  Constant: '<S2>/Constant'
   */
  rtb_Compare_p = (CarreraUdpTest_B.CCallerinput_o8 !=
                   CarreraUdpTest_P.CompareToConstant1_const);

  /* Logic: '<Root>/OR' */
  rtb_Compare = (rtb_Compare || rtb_Compare_p);

  /* DiscreteIntegrator: '<S42>/Integrator' */
  if (rtb_Compare || (CarreraUdpTest_DW.Integrator_PrevResetState != 0)) {
    CarreraUdpTest_DW.Integrator_DSTATE =
      CarreraUdpTest_P.DiscretePIDController_Initial_k;
  }

  /* DiscreteIntegrator: '<S37>/Filter' */
  if (rtb_Compare || (CarreraUdpTest_DW.Filter_PrevResetState != 0)) {
    CarreraUdpTest_DW.Filter_DSTATE =
      CarreraUdpTest_P.DiscretePIDController_InitialCo;
  }

  /* Gain: '<S45>/Filter Coefficient' incorporates:
   *  DiscreteIntegrator: '<S37>/Filter'
   *  Gain: '<S35>/Derivative Gain'
   *  Sum: '<S37>/SumD'
   */
  rtb_Gain = (CarreraUdpTest_P.DiscretePIDController_D * rtb_IntegralGain -
              CarreraUdpTest_DW.Filter_DSTATE) *
    CarreraUdpTest_P.DiscretePIDController_N;

  /* Sum: '<S51>/Sum' incorporates:
   *  DiscreteIntegrator: '<S42>/Integrator'
   *  Gain: '<S47>/Proportional Gain'
   */
  rtb_DeadZone = (CarreraUdpTest_P.DiscretePIDController_P * rtb_IntegralGain +
                  CarreraUdpTest_DW.Integrator_DSTATE) + rtb_Gain;

  /* Saturate: '<S49>/Saturation' */
  if (rtb_DeadZone > CarreraUdpTest_P.DiscretePIDController_UpperSatu) {
    rtb_Saturation = CarreraUdpTest_P.DiscretePIDController_UpperSatu;
  } else if (rtb_DeadZone < CarreraUdpTest_P.DiscretePIDController_LowerSatu) {
    rtb_Saturation = CarreraUdpTest_P.DiscretePIDController_LowerSatu;
  } else {
    rtb_Saturation = rtb_DeadZone;
  }

  /* End of Saturate: '<S49>/Saturation' */

  /* CCaller: '<Root>/C Caller batt' */
  rtb_CCallerbatt = APJ6_Car_get_batt_voltage();

  /* CCaller: '<Root>/C Caller output' incorporates:
   *  DataTypeConversion: '<Root>/Data Type Conversion1'
   */
  APJ6_Car_send_signals((real32_T)rtb_DataTypeConversion,
                        CarreraUdpTest_B.CCallerinput_o1, rtb_Saturation,
                        CarreraUdpTest_B.CCallerinput_o3, rtb_CCallerbatt);

  /* DataTypeConversion: '<Root>/Data Type Conversion' */
  rtb_Saturation = (real32_T)floor(rtb_Saturation);
  if (rtIsNaNF(rtb_Saturation) || rtIsInfF(rtb_Saturation)) {
    rtb_Saturation = 0.0F;
  } else {
    rtb_Saturation = (real32_T)fmod(rtb_Saturation, 4.294967296E+9);
  }

  /* CCaller: '<Root>/C Caller power' incorporates:
   *  DataTypeConversion: '<Root>/Data Type Conversion'
   */
  APJ6_Car_set_power(rtb_Saturation < 0.0F ? -(int32_T)(uint32_T)-rtb_Saturation
                     : (int32_T)(uint32_T)rtb_Saturation);

  /* DeadZone: '<S34>/DeadZone' */
  if (rtb_DeadZone > CarreraUdpTest_P.DiscretePIDController_UpperSatu) {
    rtb_DeadZone -= CarreraUdpTest_P.DiscretePIDController_UpperSatu;
  } else if (rtb_DeadZone >= CarreraUdpTest_P.DiscretePIDController_LowerSatu) {
    rtb_DeadZone = 0.0F;
  } else {
    rtb_DeadZone -= CarreraUdpTest_P.DiscretePIDController_LowerSatu;
  }

  /* End of DeadZone: '<S34>/DeadZone' */

  /* Gain: '<S39>/Integral Gain' */
  rtb_IntegralGain *= CarreraUdpTest_P.DiscretePIDController_I;

  /* Switch: '<S32>/Switch1' incorporates:
   *  Constant: '<S32>/Clamping_zero'
   *  Constant: '<S32>/Constant'
   *  Constant: '<S32>/Constant2'
   *  RelationalOperator: '<S32>/fix for DT propagation issue'
   */
  if (rtb_DeadZone > CarreraUdpTest_P.Clamping_zero_Value) {
    tmp = CarreraUdpTest_P.Constant_Value;
  } else {
    tmp = CarreraUdpTest_P.Constant2_Value;
  }

  /* Switch: '<S32>/Switch2' incorporates:
   *  Constant: '<S32>/Clamping_zero'
   *  Constant: '<S32>/Constant3'
   *  Constant: '<S32>/Constant4'
   *  RelationalOperator: '<S32>/fix for DT propagation issue1'
   */
  if (rtb_IntegralGain > CarreraUdpTest_P.Clamping_zero_Value) {
    tmp_0 = CarreraUdpTest_P.Constant3_Value;
  } else {
    tmp_0 = CarreraUdpTest_P.Constant4_Value;
  }

  /* Switch: '<S32>/Switch' incorporates:
   *  Constant: '<S32>/Clamping_zero'
   *  Constant: '<S32>/Constant1'
   *  Logic: '<S32>/AND3'
   *  RelationalOperator: '<S32>/Equal1'
   *  RelationalOperator: '<S32>/Relational Operator'
   *  Switch: '<S32>/Switch1'
   *  Switch: '<S32>/Switch2'
   */
  if ((CarreraUdpTest_P.Clamping_zero_Value != rtb_DeadZone) && (tmp == tmp_0))
  {
    rtb_IntegralGain = CarreraUdpTest_P.Constant1_Value;
  }

  /* Update for DiscreteIntegrator: '<S42>/Integrator' incorporates:
   *  Switch: '<S32>/Switch'
   */
  CarreraUdpTest_DW.Integrator_DSTATE += CarreraUdpTest_P.Integrator_gainval *
    rtb_IntegralGain;
  CarreraUdpTest_DW.Integrator_PrevResetState = (int8_T)rtb_Compare;

  /* Update for DiscreteIntegrator: '<S37>/Filter' incorporates:
   *  DiscreteIntegrator: '<S42>/Integrator'
   */
  CarreraUdpTest_DW.Filter_DSTATE += CarreraUdpTest_P.Filter_gainval * rtb_Gain;
  CarreraUdpTest_DW.Filter_PrevResetState = (int8_T)rtb_Compare;
}

/* Model initialize function */
void CarreraUdpTest_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize error status */
  rtmSetErrorStatus(CarreraUdpTest_M, (NULL));

  /* block I/O */
  (void) memset(((void *) &CarreraUdpTest_B), 0,
                sizeof(B_CarreraUdpTest_T));

  /* states (dwork) */
  (void) memset((void *)&CarreraUdpTest_DW, 0,
                sizeof(DW_CarreraUdpTest_T));

  /* InitializeConditions for DiscreteIntegrator: '<S42>/Integrator' */
  CarreraUdpTest_DW.Integrator_DSTATE =
    CarreraUdpTest_P.DiscretePIDController_Initial_k;
  CarreraUdpTest_DW.Integrator_PrevResetState = 0;

  /* InitializeConditions for DiscreteIntegrator: '<S37>/Filter' */
  CarreraUdpTest_DW.Filter_DSTATE =
    CarreraUdpTest_P.DiscretePIDController_InitialCo;
  CarreraUdpTest_DW.Filter_PrevResetState = 0;

  /* Outputs for Atomic SubSystem: '<Root>/Initialize Function' */
  /* StringConstant: '<S4>/String Constant' */
  strncpy(&CarreraUdpTest_B.StringConstant[0],
          &CarreraUdpTest_P.StringConstant_String[0], 255U);
  CarreraUdpTest_B.StringConstant[255] = '\x00';

  /* StringConstant: '<S4>/String Constant1' */
  strncpy(&CarreraUdpTest_B.StringConstant1[0],
          &CarreraUdpTest_P.StringConstant1_String[0], 255U);
  CarreraUdpTest_B.StringConstant1[255] = '\x00';

  /* CCaller: '<S4>/C Caller1' */
  APJ6_Car_initialize(&CarreraUdpTest_B.StringConstant[0U],
                      &CarreraUdpTest_B.StringConstant1[0U]);

  /* End of Outputs for SubSystem: '<Root>/Initialize Function' */
}

/* Model terminate function */
void CarreraUdpTest_terminate(void)
{
  /* Outputs for Atomic SubSystem: '<Root>/Terminate Function' */
  /* CCaller: '<S5>/C Caller' */
  APJ6_Car_terminate();

  /* End of Outputs for SubSystem: '<Root>/Terminate Function' */
}
