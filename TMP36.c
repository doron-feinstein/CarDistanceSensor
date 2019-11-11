/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */
 
#include "TMP36.h"

// TMP36 pin
static int tmpSensorPin = -1;

bool tmpSensorInit(int pin)
{
  // Set the pin
  tmpSensorPin = pin;

  // Return the sensor status
  return tmpSensorReady();
}

bool tmpSensorReady()
{
  return (tmpSensorPin >= 0);
}

float measureTmpV()
{
  // Get the reading from the pin
  int tempReading = analogRead(tmpSensorPin);

  // Convert to volts
  // Since analogRead returns 0-1023 and the power source is 5V the reading needs to be normalized and multiplied by 5
  return tempReading * 5.0f / 1024.0f; 
}

bool measureTmpC(float* measuredTempC)
{
  if (measuredTempC == 0)
  {
    return false;
  }
  
  // Read the sensor voltage
  float tempVolts = measureTmpV();

  // Convert volts to temprature in celsius
  float tempC = (tempVolts - 0.5f) * 10.0f;

  // Store the measured temprature
  *measuredTempC = tempC;
  
  // Indicate an error when the temprature is out of range
  return (tempC >= -40.0f && tempC <= 125.0f);
}

bool measureTmpF(float* measuredTempF)
{
  if (measuredTempF == 0)
  {
    return false;
  }
  
  // Get temprature in celsius
  float tempC = 0.0f;
  bool res = measureTmpC(&tempC);

  if (res)
  {
    // Convert celsius to fahrenheit
    *measuredTempF = tempC * 1.8f + 32.0f;
  }
  
  return res;
}
