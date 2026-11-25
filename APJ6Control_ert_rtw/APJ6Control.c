/*
 * APJ6Control.c
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
#include "rtwtypes.h"
#include <string.h>
#include "rt_nonfinite.h"
#include <math.h>
#define APJ6Control_period             (0.05F)

/* Block signals (default storage) */
B_APJ6Control_T APJ6Control_B;

/* Block states (default storage) */
DW_APJ6Control_T APJ6Control_DW;

/* Real-time model */
static RT_MODEL_APJ6Control_T APJ6Control_M_;
RT_MODEL_APJ6Control_T *const APJ6Control_M = &APJ6Control_M_;

/* Model step function */
void APJ6Control_step(void)
{
  int32_T exitg1;
  int32_T k;
  int32_T rtb_DataTypeConversion;
  real32_T xPacked[3];
  real32_T d_med;
  real32_T gap;
  real32_T rtb_CCallerbatt;
  real32_T rtb_FilterCoefficient;
  real32_T rtb_Saturation;
  int8_T tmp;
  int8_T tmp_0;
  boolean_T rtb_Compare;
  boolean_T seen;

  /* CCaller: '<Root>/C Caller input' */
  APJ6_Car_receive_signals(&APJ6Control_B.CCallerinput_o1,
    &APJ6Control_B.CCallerinput_o2, &APJ6Control_B.CCallerinput_o3,
    &APJ6Control_B.CCallerinput_o4, &APJ6Control_B.CCallerinput_o5,
    &APJ6Control_B.CCallerinput_o6, &APJ6Control_B.CCallerinput_o7,
    &APJ6Control_B.CCallerinput_o8);

  /* RelationalOperator: '<S2>/Compare' incorporates:
   *  Constant: '<S2>/Constant'
   */
  rtb_Compare = (APJ6Control_B.CCallerinput_o8 !=
                 APJ6Control_P.CompareToConstant1_const);

  /* CCaller: '<Root>/C Caller distance' */
  rtb_DataTypeConversion = APJ6_Car_get_distance();

  /* CCaller: '<Root>/C Caller' */
  APJ6Control_B.CCaller = APJ6_Car_get_speed();

  /* DiscreteFir: '<Root>/Discrete FIR Filter' */
  APJ6Control_B.DiscreteFIRFilter = APJ6Control_B.CCaller *
    APJ6Control_P.DiscreteFIRFilter_Coefficients[0] +
    APJ6Control_DW.DiscreteFIRFilter_states *
    APJ6Control_P.DiscreteFIRFilter_Coefficients[1];

  /* MATLAB Function: '<Root>/MATLAB Function1' incorporates:
   *  DataTypeConversion: '<Root>/Data Type Conversion1'
   */
  xPacked[0] = (real32_T)rtb_DataTypeConversion;
  xPacked[1] = APJ6Control_DW.d1;
  xPacked[2] = APJ6Control_DW.d2;
  k = 0;
  do {
    exitg1 = 0;
    if (k < 3) {
      if (rtIsNaNF(xPacked[k])) {
        d_med = (rtNaNF);
        exitg1 = 1;
      } else {
        k++;
      }
    } else {
      if (rtb_DataTypeConversion < APJ6Control_DW.d1) {
        if (APJ6Control_DW.d1 < APJ6Control_DW.d2) {
          k = 1;
        } else if (rtb_DataTypeConversion < APJ6Control_DW.d2) {
          k = 2;
        } else {
          k = 0;
        }
      } else if (rtb_DataTypeConversion < APJ6Control_DW.d2) {
        k = 0;
      } else if (APJ6Control_DW.d1 < APJ6Control_DW.d2) {
        k = 2;
      } else {
        k = 1;
      }

      d_med = xPacked[k];
      exitg1 = 1;
    }
  } while (exitg1 == 0);

  APJ6Control_DW.d2 = APJ6Control_DW.d1;
  APJ6Control_DW.d1 = (real32_T)rtb_DataTypeConversion;
  if (d_med < 1400.0F) {
    if (APJ6Control_DW.count + 1.0 <= 100.0) {
      APJ6Control_DW.count++;
    } else {
      APJ6Control_DW.count = 100.0;
    }
  } else {
    APJ6Control_DW.count = 0.0;
  }

  seen = (APJ6Control_DW.count >= 3.0);
  if (seen && (APJ6Control_DW.count > 3.0)) {
    APJ6Control_DW.rel_f += ((d_med - APJ6Control_DW.dprev) / 0.05F -
      APJ6Control_DW.rel_f) * 0.3F;
  } else {
    APJ6Control_DW.rel_f = 0.0F;
  }

  APJ6Control_DW.dprev = d_med;
  gap = APJ6Control_B.CCallerinput_o2;
  if (APJ6Control_B.CCallerinput_o2 <= 0.0F) {
    gap = 400.0F;
  }

  if (seen) {
    gap = (APJ6Control_B.DiscreteFIRFilter + APJ6Control_DW.rel_f) + (d_med -
      gap);
    if (!(gap >= 0.0F)) {
      gap = 0.0F;
    }

    rtb_FilterCoefficient = APJ6Control_B.CCallerinput_o1;
    if (rtb_FilterCoefficient <= gap) {
      gap = rtb_FilterCoefficient;
    }

    if (d_med < 120.0F) {
      gap = 0.0F;
    }
  } else {
    gap = APJ6Control_B.CCallerinput_o1;
  }

  /* RateLimiter: '<Root>/Rate Limiter1' */
  rtb_FilterCoefficient = gap - APJ6Control_DW.PrevY;
  if (rtb_FilterCoefficient > (real32_T)APJ6Control_P.RateLimiter1_RisingLim *
      APJ6Control_period) {
    gap = (real32_T)APJ6Control_P.RateLimiter1_RisingLim * APJ6Control_period +
      APJ6Control_DW.PrevY;
  } else if (rtb_FilterCoefficient < (real32_T)
             APJ6Control_P.RateLimiter1_FallingLim * APJ6Control_period) {
    gap = (real32_T)APJ6Control_P.RateLimiter1_FallingLim * APJ6Control_period +
      APJ6Control_DW.PrevY;
  }

  APJ6Control_DW.PrevY = gap;

  /* End of RateLimiter: '<Root>/Rate Limiter1' */

  /* Logic: '<Root>/OR' incorporates:
   *  Constant: '<S1>/Constant'
   *  RelationalOperator: '<S1>/Compare'
   */
  rtb_Compare = ((gap <= APJ6Control_P.CompareToConstant_const) || rtb_Compare);

  /* DiscreteIntegrator: '<S43>/Integrator' */
  if (rtb_Compare || (APJ6Control_DW.Integrator_PrevResetState != 0)) {
    APJ6Control_DW.Integrator_DSTATE =
      APJ6Control_P.DiscretePIDController_Initial_k;
  }

  /* Sum: '<Root>/Sum' */
  gap -= APJ6Control_B.DiscreteFIRFilter;

  /* DiscreteIntegrator: '<S38>/Filter' */
  if (rtb_Compare || (APJ6Control_DW.Filter_PrevResetState != 0)) {
    APJ6Control_DW.Filter_DSTATE = APJ6Control_P.DiscretePIDController_InitialCo;
  }

  /* Gain: '<S46>/Filter Coefficient' incorporates:
   *  DiscreteIntegrator: '<S38>/Filter'
   *  Gain: '<S36>/Derivative Gain'
   *  Sum: '<S38>/SumD'
   */
  rtb_FilterCoefficient = (APJ6Control_P.DiscretePIDController_D * gap -
    APJ6Control_DW.Filter_DSTATE) * APJ6Control_P.DiscretePIDController_N;

  /* Sum: '<S52>/Sum' incorporates:
   *  DiscreteIntegrator: '<S43>/Integrator'
   *  Gain: '<S48>/Proportional Gain'
   */
  rtb_Saturation = (APJ6Control_P.DiscretePIDController_P * gap +
                    APJ6Control_DW.Integrator_DSTATE) + rtb_FilterCoefficient;

  /* DeadZone: '<S35>/DeadZone' */
  if (rtb_Saturation > APJ6Control_P.DiscretePIDController_UpperSatu) {
    rtb_CCallerbatt = rtb_Saturation -
      APJ6Control_P.DiscretePIDController_UpperSatu;
  } else if (rtb_Saturation >= APJ6Control_P.DiscretePIDController_LowerSatu) {
    rtb_CCallerbatt = 0.0F;
  } else {
    rtb_CCallerbatt = rtb_Saturation -
      APJ6Control_P.DiscretePIDController_LowerSatu;
  }

  /* End of DeadZone: '<S35>/DeadZone' */

  /* Gain: '<S40>/Integral Gain' */
  gap *= APJ6Control_P.DiscretePIDController_I;

  /* Switch: '<S33>/Switch1' incorporates:
   *  Constant: '<S33>/Clamping_zero'
   *  Constant: '<S33>/Constant'
   *  Constant: '<S33>/Constant2'
   *  RelationalOperator: '<S33>/fix for DT propagation issue'
   */
  if (rtb_CCallerbatt > APJ6Control_P.Clamping_zero_Value) {
    tmp = APJ6Control_P.Constant_Value;
  } else {
    tmp = APJ6Control_P.Constant2_Value;
  }

  /* Switch: '<S33>/Switch2' incorporates:
   *  Constant: '<S33>/Clamping_zero'
   *  Constant: '<S33>/Constant3'
   *  Constant: '<S33>/Constant4'
   *  RelationalOperator: '<S33>/fix for DT propagation issue1'
   */
  if (gap > APJ6Control_P.Clamping_zero_Value) {
    tmp_0 = APJ6Control_P.Constant3_Value;
  } else {
    tmp_0 = APJ6Control_P.Constant4_Value;
  }

  /* Switch: '<S33>/Switch' incorporates:
   *  Constant: '<S33>/Clamping_zero'
   *  Constant: '<S33>/Constant1'
   *  Logic: '<S33>/AND3'
   *  RelationalOperator: '<S33>/Equal1'
   *  RelationalOperator: '<S33>/Relational Operator'
   *  Switch: '<S33>/Switch1'
   *  Switch: '<S33>/Switch2'
   */
  if ((APJ6Control_P.Clamping_zero_Value != rtb_CCallerbatt) && (tmp == tmp_0))
  {
    gap = APJ6Control_P.Constant1_Value;
  }

  /* End of Switch: '<S33>/Switch' */

  /* Saturate: '<S50>/Saturation' */
  if (rtb_Saturation > APJ6Control_P.DiscretePIDController_UpperSatu) {
    rtb_Saturation = APJ6Control_P.DiscretePIDController_UpperSatu;
  } else if (rtb_Saturation < APJ6Control_P.DiscretePIDController_LowerSatu) {
    rtb_Saturation = APJ6Control_P.DiscretePIDController_LowerSatu;
  }

  /* End of Saturate: '<S50>/Saturation' */

  /* CCaller: '<Root>/C Caller batt' */
  rtb_CCallerbatt = APJ6_Car_get_batt_voltage();

  /* CCaller: '<Root>/C Caller output' incorporates:
   *  MATLAB Function: '<Root>/MATLAB Function1'
   */
  APJ6_Car_send_signals(d_med, APJ6Control_B.CCallerinput_o1, rtb_Saturation,
                        APJ6Control_B.CCallerinput_o3, rtb_CCallerbatt);

  /* DataTypeConversion: '<Root>/Data Type Conversion' */
  d_med = (real32_T)floor(rtb_Saturation);
  if (rtIsNaNF(d_med) || rtIsInfF(d_med)) {
    d_med = 0.0F;
  } else {
    d_med = (real32_T)fmod(d_med, 4.294967296E+9);
  }

  /* CCaller: '<Root>/C Caller power' incorporates:
   *  DataTypeConversion: '<Root>/Data Type Conversion'
   */
  APJ6_Car_set_power(d_med < 0.0F ? -(int32_T)(uint32_T)-d_med : (int32_T)
                     (uint32_T)d_med);

  /* Update for DiscreteFir: '<Root>/Discrete FIR Filter' */
  /* Update states */
  APJ6Control_DW.DiscreteFIRFilter_states = APJ6Control_B.CCaller;

  /* Update for DiscreteIntegrator: '<S43>/Integrator' */
  APJ6Control_DW.Integrator_DSTATE += APJ6Control_P.Integrator_gainval * gap;
  APJ6Control_DW.Integrator_PrevResetState = (int8_T)rtb_Compare;

  /* Update for DiscreteIntegrator: '<S38>/Filter' incorporates:
   *  DiscreteIntegrator: '<S43>/Integrator'
   */
  APJ6Control_DW.Filter_DSTATE += APJ6Control_P.Filter_gainval *
    rtb_FilterCoefficient;
  APJ6Control_DW.Filter_PrevResetState = (int8_T)rtb_Compare;
}

/* Model initialize function */
void APJ6Control_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize error status */
  rtmSetErrorStatus(APJ6Control_M, (NULL));

  /* block I/O */
  (void) memset(((void *) &APJ6Control_B), 0,
                sizeof(B_APJ6Control_T));

  /* states (dwork) */
  (void) memset((void *)&APJ6Control_DW, 0,
                sizeof(DW_APJ6Control_T));

  /* InitializeConditions for DiscreteFir: '<Root>/Discrete FIR Filter' */
  APJ6Control_DW.DiscreteFIRFilter_states =
    APJ6Control_P.DiscreteFIRFilter_InitialStates;

  /* InitializeConditions for RateLimiter: '<Root>/Rate Limiter1' */
  APJ6Control_DW.PrevY = APJ6Control_P.RateLimiter1_IC;

  /* InitializeConditions for DiscreteIntegrator: '<S43>/Integrator' */
  APJ6Control_DW.Integrator_DSTATE =
    APJ6Control_P.DiscretePIDController_Initial_k;
  APJ6Control_DW.Integrator_PrevResetState = 0;

  /* InitializeConditions for DiscreteIntegrator: '<S38>/Filter' */
  APJ6Control_DW.Filter_DSTATE = APJ6Control_P.DiscretePIDController_InitialCo;
  APJ6Control_DW.Filter_PrevResetState = 0;

  /* SystemInitialize for MATLAB Function: '<Root>/MATLAB Function1' */
  APJ6Control_DW.d1 = 1500.0F;
  APJ6Control_DW.d2 = 1500.0F;
  APJ6Control_DW.dprev = 1500.0F;
  APJ6Control_DW.rel_f = 0.0F;
  APJ6Control_DW.count = 0.0;

  /* Outputs for Atomic SubSystem: '<Root>/Initialize Function' */
  /* StringConstant: '<S4>/String Constant' */
  strncpy(&APJ6Control_B.StringConstant[0],
          &APJ6Control_P.StringConstant_String[0], 255U);
  APJ6Control_B.StringConstant[255] = '\x00';

  /* StringConstant: '<S4>/String Constant1' */
  strncpy(&APJ6Control_B.StringConstant1[0],
          &APJ6Control_P.StringConstant1_String[0], 255U);
  APJ6Control_B.StringConstant1[255] = '\x00';

  /* CCaller: '<S4>/C Caller1' */
  APJ6_Car_initialize(&APJ6Control_B.StringConstant[0U],
                      &APJ6Control_B.StringConstant1[0U]);

  /* End of Outputs for SubSystem: '<Root>/Initialize Function' */
}

/* Model terminate function */
void APJ6Control_terminate(void)
{
  /* Outputs for Atomic SubSystem: '<Root>/Terminate Function' */
  /* CCaller: '<S6>/C Caller' */
  APJ6_Car_terminate();

  /* End of Outputs for SubSystem: '<Root>/Terminate Function' */
}
