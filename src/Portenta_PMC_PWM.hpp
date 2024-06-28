#pragma once

#ifndef Portenta_H7_PWM_HPP
#define Portenta_H7_PWM_HPP

#if ( ( defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_H7_M4) ) && defined(ARDUINO_ARCH_MBED) )
  #warning Use MBED ARDUINO_PORTENTA_H7 and LittleFS
#else
  #error This code is intended to run on the MBED ARDUINO_PORTENTA_H7 platform! Please check your Tools->Board setting. 
#endif

#ifndef PORTENTA_H7_PWM_VERSION
  #define PORTENTA_H7_PWM_VERSION             "Portenta_H7_PWM v2.1.0"
  
  #define PORTENTA_H7_PWM_VERSION_MAJOR       2
  #define PORTENTA_H7_PWM_VERSION_MINOR       1
  #define PORTENTA_H7_PWM_VERSION_PATCH       0

  #define PORTENTA_H7_PWM_VERSION_INT         2001000
#endif


///////////////////////////////////////////

#include "Arduino.h"
#include "pinDefinitions.h"
#include "PwmOut.h"

///////////////////////////////////////////
  
#if defined(BOARD_NAME)
  #undef BOARD_NAME
#endif

#if defined(CORE_CM7)
  #warning Using Portenta H7 M7 core
  #define BOARD_NAME              "PORTENTA_H7_M7"
#else
  #warning Using Portenta H7 M4 core
  #define BOARD_NAME              "PORTENTA_H7_M4"
#endif
  
///////////////////////////////////////////

#ifndef _PWM_LOGLEVEL_
  #define _PWM_LOGLEVEL_       1
#endif

#include "PWM_Generic_Debug.h"

///////////////////////////////////////////

bool isValidPWMPin(const pin_size_t& pin);

bool isValidPWMFreq(const pin_size_t& pin, const float& frequency);

bool isValidPWMDutyCycle(const pin_size_t& pin, const float& dutyCycle);

bool isUsingHRTIM(const pin_size_t& pin);

bool isValidPWMSettings(const pin_size_t& pin, const float& frequency, const float& dutyCycle);

mbed::PwmOut* setPWM(mbed::PwmOut* &pwm, const pin_size_t& pin, const float& frequency, const float& dutyCycle);

mbed::PwmOut* setPWM_DCPercentage_manual(mbed::PwmOut* &pwm, const pin_size_t& pin, const float& DCPercentage);

mbed::PwmOut* stopPWM(mbed::PwmOut* &pwm, const pin_size_t& pin);

#endif    // Portenta_H7_PWM_HPP

