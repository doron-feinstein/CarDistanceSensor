/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#ifndef __CARDISTANCESENSOR_H_11_13_2020__
#define __CARDISTANCESENSOR_H_11_13_2020__

#include "DistanceSensorSEN0311.h"
#include "LEDControl.h"

/**
 * 
 */
class CarDistanceSensor
{
public:
  CarDistanceSensor();

  /**
   * Initialize the sensors
   * @return True if the system is ready, otherwise false
   */
  bool init();

  /**
   * Get a distance sensor reading and update the display to indicate that distance
   */
  void update();

private:

  /**
   * Update the display based on the given distance
   * @param Distance value
   */
  void updateDisplay(unsigned int dist);

  /**
   * Disable unused functionality of the ATMega to save some power
   */
  void initPowerSaving();

  /**
   * Send the ATMega to sleep to save power
   * Stay asleep as long as the distance reading is in the same range
   */
  void sendToSleep();

private:

  /**
   * Distance sensor
   */
  DistanceSensorSEN0311 _distanceSensor;

  /**
   * 8 distance indicating LEDs controller
   */
  LEDControl _LEDCtl;

  /**
   * Status LED
   * On when system is working correctly
   * Blinks when there is an error
   */
  LEDControl _statusLED;

  /**
   * Last measured time
   */
  unsigned long _lastTime;

  /**
   * Current stable distance
   */
  unsigned int _stableDistance;

  /**
   * Currnt amount of time the distance was stable
   */
  unsigned long _stableDistanceTime;

  /**
   * Time of the first error after a good reading
   */
  unsigned long _errorStartTime;
};

#endif // __CARDISTANCESENSOR_H_11_13_2020__
