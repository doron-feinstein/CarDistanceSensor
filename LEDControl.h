/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#ifndef __LED_CONTROL_H_11_25_2020__
#define __LED_CONTROL_H_11_25_2020__

#include <Arduino.h>

/**
 * Control up to 8 LEDs that can be turned on / off individually or blink
 * LEDs can be driven directly with digital pins or through a shift register to reduce the amount of pins used
 */
class LEDControl
{
public:

  /**
   * Construct with shift register
   * @param Number of lights to control
   * @param Shift register data pin
   * @param Shift register clock pin
   * @param Shift register latch pin
   */
  LEDControl(int numLights, int dataPin, int clockPin, int latchPin);
  
  /**
   * Construct with direct LED drive
   * @param Number of lights to control
   * @param Array with pin number for each light
   */
  LEDControl(int numLights, const int ledPins[]);
  
  /**
   * Flash all LEDs at a rate of 3Hz
   */
  void flashLights();

  /**
   * Update the LED state with a given bit field
   * @param byte with a a bit state for each LED (0 off and 1 on)
   */
  void updateLights(byte b);
  
private:

  /**
   * Number of lights controlled
   */
  int _numLights;

  /**
   * LED direct drive pins
   */
  int _ledPins[8];

  /**
   * Shift register data pin
   */
  int _dataPin;

  /**
   * Shift register clock pin
   */
  int _clockPin;

  /**
   * Shift register latch pin
   */
  int _latchPin;
  
  /**
   * True when a shift register is used otherwise false
   */
  bool _usingShiftRegister;

  /**
   * Internal timer for the light flashing
   */
  unsigned long _lastUpdateTime;
};

#endif // __LED_CONTROL_H_11_25_2020__
