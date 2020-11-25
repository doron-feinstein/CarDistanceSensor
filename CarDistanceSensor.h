/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#ifndef __CARDISTANCESENSOR_H_11_13_2020__
#define __CARDISTANCESENSOR_H_11_13_2020__

#if USE_DIST_SENS_SEN0311
#include "DistanceSensorSEN0311.h"
#else
#include "DistanceSensorHCSR04.h"
#endif // USE_DIST_SENS_SEN0311

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
   * Distance sensor
   */
#if USE_DIST_SENS_SEN0311
  DistanceSensorSEN0311 _distanceSensor;
#else
  DistanceSensorHCSR04 _distanceSensor;
#endif // USE_DIST_SENS_SEN0311
};

#endif // __CARDISTANCESENSOR_H_11_13_2020__
