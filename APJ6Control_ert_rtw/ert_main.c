#include "APJ6Control.h"
#include "rtwtypes.h"
#include "MW_target_hardware_resources.h"

volatile int IsrOverrun = 0;
static boolean_T OverrunFlag = 0;
void rt_OneStep(void)
{
  /* Check for overrun. Protect OverrunFlag against preemption */
  if (OverrunFlag++) {
    IsrOverrun = 1;
    OverrunFlag--;
    return;
  }

#ifndef _MW_ARDUINO_LOOP_

  interrupts();

#endif;

  APJ6Control_step();

  /* Get model outputs here */
#ifndef _MW_ARDUINO_LOOP_

  noInterrupts();

#endif;

  OverrunFlag--;
}

volatile boolean_T stopRequested;
volatile boolean_T runModel;
int main(void)
{
  float modelBaseRate = 0.05;
  float systemClock = 0;

  /* Initialize variables */
  stopRequested = false;
  runModel = false;
  init();
  MW_usbattach();
  MW_Arduino_Init();
  rtmSetErrorStatus(APJ6Control_M, 0);
  APJ6Control_initialize();
  noInterrupts();
  configureArduinoARM_M0plusTimer();
  runModel =
    rtmGetErrorStatus(APJ6Control_M) == (NULL);

#ifndef _MW_ARDUINO_LOOP_

  interrupts();

#endif;

  interrupts();
  while (runModel) {
    stopRequested = !(
                      rtmGetErrorStatus(APJ6Control_M) == (NULL));
    MW_OTA_Poll();
    MW_Arduino_Loop();
    MW_Modbus_Slave_Poll();
  }

  /* Terminate model */
  APJ6Control_terminate();
  MW_Arduino_Terminate();
  noInterrupts();
  return 0;
}
