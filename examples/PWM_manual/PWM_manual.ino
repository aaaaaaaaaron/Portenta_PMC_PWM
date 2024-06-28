/****************************************************************************************************************************
  PWM_manual.ino
  For Portenta_H7 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/Portenta_H7_PWM
  Licensed under MIT license
*****************************************************************************************************************************/

#include <Arduino_PortentaMachineControl.h>


#if !( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) )
  #error For Portenta_H7 only
#endif

#define _PWM_LOGLEVEL_        4

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "Portenta_PMC_PWM.h"

#define UPDATE_INTERVAL       1000//1000L

// Using setPWM_DCPercentage_manual if true
#define USING_DC_PERCENT      true

// Can't use same TimerIndex again, e.g., the D1 and D2, using TIM1, can't be use concurrently
// That's why D0, D1, D3, D4 and D6 (using TimerIndex 8, 1, HRTIM and 3) are OK together

#define pinD0    MC_DO_DO0_PIN // works
#define pinD1    MC_DO_DO1_PIN // works
#define pinD2    MC_DO_DO2_PIN // works
#define pinD3    MC_DO_DO3_PIN // does not work
#define pinD4    MC_DO_DO4_PIN // does not work
#define pinD5    MC_DO_DO5_PIN // works
#define pinD6    MC_DO_DO6_PIN // does not work
#define pinD7    MC_DO_DO7_PIN // does not work


// See https://www.st.com/resource/en/datasheet/stm32h747xi.pdf, Table 7, page 53
// Can't use pins with same TIMx. For example,
// pinD1 and pinD2, using same TIM1, can't be used at the same time
// pinD4 and pinD5, using same TIM3, can't be used at the same time
// pinD3 and pinD6 are using HRTIM, can't be used at the same time and the minimum freq must be ~770Hz

uint32_t myPin  = pinD2;

mbed::PwmOut* pwm   = nullptr;

float    frequency = 300.0f;

float    dutycyclePercent = 0.0f;
float    DCStepPercent    = 20.0f;

void setup()
{

  Serial.begin(115200);
  Wire.begin(); //todo I think I can remove this
  while (!Serial && millis() < 5000);

  delay(100);
  Serial.println();
  Serial.println("starting io -+-+");

  MachineControl_DigitalOutputs.begin(true);

  Serial.println("Sigital I/Os initialized successfully!");

  Serial.print(F("\nStarting PWM_manual on "));
  Serial.println(BOARD_NAME);
  Serial.println(PORTENTA_H7_PWM_VERSION);


  setPWM(pwm, (PinName) myPin, frequency, 10);
}

void loop()
{
  static unsigned long update_timeout = UPDATE_INTERVAL;

  // Update DC every UPDATE_INTERVAL (1000) milliseconds
  if (millis() > update_timeout)
  {
    setPWM_DCPercentage_manual(pwm, myPin, dutycyclePercent);

    dutycyclePercent += DCStepPercent;

    if (dutycyclePercent > 100.0f)
      dutycyclePercent = 0.0f;
   
    update_timeout = millis() + UPDATE_INTERVAL;
  }
}