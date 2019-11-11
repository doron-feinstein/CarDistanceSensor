/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#ifndef __HCSR04_H_11_10_2019__
#define __HCSR04_H_11_10_2019__

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the distance sensor
 * @param echo pin the sensor is connected to
 * @param trigger pin the sensor is connected to
 */
void distSensorInit(int echoPin, int trigPin);

/**
 * Measure distance in centimeters
 * @param optional temprature value for compensation (zero for no compensation)
 * @param pointer to store the measured distance
 */
bool measureDist(float tempC, unsigned int* dist);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __HCSR04_H_11_10_2019__
