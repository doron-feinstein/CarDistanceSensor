/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#ifndef __LED_CONTROL_H_11_25_2020__
#define __LED_CONTROL_H_11_25_2020__

#include <Arduino.h>

class LEDControl
{
public:

  LEDControl();

  /**
   * Flash all LEDs at a rate of 3Hz
   */
  void flashLights();

  /**
   * Update the LED state with a given bit field
   * @param byte with a a bit state for each LED (0 off and 1 on)
   */
  void writeByte(byte b);
  
private:

  /**
   * Internal timer for the light flashing
   */
  unsigned long _lastUpdateTime;
};

#endif // __LED_CONTROL_H_11_25_2020__
