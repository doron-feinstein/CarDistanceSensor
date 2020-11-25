/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#ifndef __DISTANCESENSOR_HCSR04_H_11_13_2020__
#define __DISTANCESENSOR_HCSR04_H_11_13_2020__

/**
 * 
 */
class DistanceSensorHCSR04
{
public:

  /**
   * Construct with given pins
   */
  DistanceSensorHCSR04(int echoPin, int trigPin);

  /**
   * Get a measurement reading from the sensor
   * @param Temprature in C for compensation
   * @param Reference for the distance measured in centimeters (only updated when reading was successfully taken)
   * @return True if measurement was taken successfully, otherwise false
   */
  bool getReading(float tempC, unsigned int& dist) const;

private:

  /**
   * Pin for triggering a distance measurement
   */
  int _trigPin;
  
  /**
   * Pin for the echo return time measurement
   */
  int _echoPin;

  /**
   * Timeout in microseconds for sensor read
   * This value is roughly the time to measure 400cm which is the maximum needed for this project
   * The value was calculated for temprature of 40c 
   */
  const unsigned long _timeout = 14063L;//150000L
  
  /**
   * Maximum measurable distance in millimeters (400mm according to spec)
   */
  const unsigned int _maxDistance = 400;
};

#endif // __DISTANCESENSOR_HCSR04_H_11_13_2020__
