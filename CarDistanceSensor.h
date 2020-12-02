/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#ifndef __CARDISTANCESENSOR_H_11_13_2020__
#define __CARDISTANCESENSOR_H_11_13_2020__

#if USE_DIST_SENS_SEN0311
#include "DistanceSensorSEN0311.h"
#else
#include "TMP36.h"
#include "DistanceSensorHCSR04.h"
#endif // USE_DIST_SENS_SEN0311

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
   * @param True for debug printouts
   * @return True if the system is ready, otherwise false
   */
  bool init(bool debug);

  /**
   * 
   */
  void update();

private:

  /**
   * Update the display based on the given distance
   * @param Distance value
   */
  void updateDisplay(unsigned int dist);

  /**
   * Distance sensor
   */
#if USE_DIST_SENS_SEN0311
  DistanceSensorSEN0311 _distanceSensor;
#else
  DistanceSensorHCSR04 _distanceSensor;
#endif // USE_DIST_SENS_SEN0311

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
};

#endif // __CARDISTANCESENSOR_H_11_13_2020__
