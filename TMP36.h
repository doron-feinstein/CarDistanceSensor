/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#ifndef __TMP36_H_11_10_2019__
#define __TMP36_H_11_10_2019__

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the temprature sensor
 * @param sensor analog pin number
 * @return true if sensor is ready otherwise false
 */
bool tmpSensorInit(int pin);

/**
 * Check if the temprature sensor is ready for use
 * @return true if sensor is ready, otherwise false
 */
bool tmpSensorReady();

/**
 * Measure temprature in celcius
 * @param pointer to store the measured temprature
 * @return true if measurement is valid, otherwise false
 */
bool measureTmpC(float* measuredTempC);

/**
 * Measure temprature in fahrenheit
 * @param pointer to store the measured temprature
 * @return true if measurement is valid, otherwise false
 */
bool measureTmpF(float* measuredTempF);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __TMP36_H_11_10_2019__
