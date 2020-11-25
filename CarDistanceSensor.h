/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#ifndef __CARDISTANCESENSOR_H_11_13_2020__
#define __CARDISTANCESENSOR_H_11_13_2020__

#include "DistanceSensorHCSR04.h"

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
  DistanceSensorHCSR04 _distanceSensor;
};

#endif // __CARDISTANCESENSOR_H_11_13_2020__
