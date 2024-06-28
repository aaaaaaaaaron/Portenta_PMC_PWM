#ifndef Portenta_PMC_PWM_IMPL_H
#define Portenta_PMC_PWM_IMPL_H

///////////////////////////////////////////

#ifndef _PWM_LOGLEVEL_
  #define _PWM_LOGLEVEL_       1
#endif

#include "Arduino.h"
#include "Portenta_PMC_PWM.h"

#include "PWM_Generic_Debug.h"


///////////////////////////////////////////

bool isUsingHRTIM(const pin_size_t& pin) // todo is this accurate? What does this even mean?
{
  if ( (pin == D3) || (pin == D6) )
  {
    PWM_LOGDEBUG1("Using HRTIM pin = ", pin);

    return true;
  }

  return false;
}

///////////////////////////////////////////

// Will calculate accurate min freq if necessary, depending on Portenta Clock
#define MIN_HRTIM_PWM_FREQ      800

///////////////////////////////////////////

bool isValidPWMSettings(const PinName& pin, const float& frequency, const float& dutyCycle)
{
  if ( (frequency <= 0) || ( isUsingHRTIM(pin) && ( frequency < MIN_HRTIM_PWM_FREQ ) ) )
  {
    if (isUsingHRTIM(pin))
    {
      PWM_LOGERROR5("Bad HRTIM Freq = ", frequency, ", pin = ", pin, ", must be >= ", MIN_HRTIM_PWM_FREQ);
    }
    else
    {
      PWM_LOGERROR3("Bad Freq = ", frequency, ", pin = ", pin);
    }

    return false;
  }
  else if ( (dutyCycle < 0.0f) || (dutyCycle > 100.0f) )
  {
    PWM_LOGERROR3("Bad dutyCycle = ", dutyCycle, ", pin = ", pin);

    return false;
  }
  else if ( !( (pin == MC_DO_DO0_PIN) || (pin == MC_DO_DO1_PIN) || (pin == MC_DO_DO2_PIN) || (pin == MC_DO_DO5_PIN) ) )
  {
    PWM_LOGDEBUG1("Pin must be either digital out pin 0, 1, 2, or 5. Selected pin #: ", pin);

    return false;
  }

  return true;
}

///////////////////////////////////////////

// dutyCycle from 0.0f to 100.0f
mbed::PwmOut* setPWM(mbed::PwmOut* &pwm, const PinName& pin, const float& frequency, const float& dutyCycle)
{
  PWM_LOGDEBUG7("Freq = ", frequency, ", \tDutyCycle % = ", dutyCycle, ", \tDutyCycle = ", dutyCycle / 100, ", \tPin = ",
                pin);

  if ( !isValidPWMSettings(pin, frequency, dutyCycle) )
  {
    return NULL;
  }

  float percent = (float) dutyCycle / 100;


  pwm = new mbed::PwmOut(pin);
  pwm->period_us( 1000000.0f / frequency );
  pwm->write(percent);


  //Warning: I think not using this code might lead to some issues. Handle this logic in sketch.
  // if (digitalPinToPwm(pin) == NULL)
  // {
  //   PWM_LOGDEBUG("New pwm");

  //   pwm = new mbed::PwmOut(digitalPinToPinName(pin));

  //   digitalPinToPwm(pin) = pwm;
    

  //   pwm->period_us( 1000000.0f / frequency );

  //   pwm->write(percent);
  // }
  // else if (pwm && (digitalPinToPwm(pin) == pwm) )
  // {
  //   //PWM_LOGDEBUG("Use existing pwm");

  //   if ( frequency != 1000000.0f / pwm->read_period_us())
  //   {
  //     PWM_LOGDEBUG3("New Freq =", frequency, ", old freq =", 1000000.0f / pwm->read_period_us());
  //     pwm->period_us( 1000000.0f / frequency );
  //   }  

  //   pwm->write(percent);
  // }

  return pwm;
}

///////////////////////////////////////////

// Must have same pin and frequency    
// DCPercentage from 0.0f - 100.0f
mbed::PwmOut* setPWM_DCPercentage_manual(mbed::PwmOut* &pwm, const pin_size_t& pin, const float& DCPercentage)
{ 
  PWM_LOGDEBUG5("DCPercentage % = ", DCPercentage, ", \tDutyCycle = ", DCPercentage / 100, ", \tPin = ", pin);
                
  pwm->write(DCPercentage / 100);
  
  return pwm;
}

///////////////////////////////////////////
    
mbed::PwmOut* stopPWM(mbed::PwmOut* &pwm, const pin_size_t& pin)
{
  if (isUsingHRTIM(pin))
  {
    // Check https://github.com/arduino/ArduinoCore-mbed/issues/352
    // Will remove when the bug is fixed
    return setPWM(pwm, pin, 1000.0f, 100.0f);
  }
  else
  {
    return setPWM(pwm, pin, 1000.0f, 0.0f);
  }
}

#endif    // Portenta_PMC_PWM_IMPL_H


