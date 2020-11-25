/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#include <Arduino.h>
#include "DistanceSensorHCSR04.h"

DistanceSensorHCSR04::DistanceSensorHCSR04(int trigPin, int echoPin)
: _trigPin(trigPin)
, _echoPin(echoPin)
{
  // Initizlize the trigger pint to ouptut and set to low
  pinMode(_trigPin, OUTPUT);
  digitalWrite(_trigPin, LOW); // Start low
  
  // Initilize the echo pin to input
  pinMode(_echoPin, INPUT);
}

bool DistanceSensorHCSR04::getReading(float tempC, unsigned int& dist) const
{
  // Trigger the sensor by sending a 10 microseconds pulse to the sensor
  digitalWrite(_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(_trigPin, LOW);

  // Measure the response pulse duration in microseconds
  unsigned long duration = pulseIn(_echoPin, HIGH, _timeout);

  // Duration should never be zero
  if (duration == 0)
  {
    Serial.print("Duration is zero - trig ");
    Serial.print(_trigPin);
    Serial.print(" echo ");
    Serial.print(_echoPin);
    Serial.println("");
    return false;
  }
  
  // Convert duration in microseconds to distance in centimeters
  // The measurement is time compensated based on the temprature
  // Time needs to be devided by two to account for the time to the object and back
  dist = (duration * (331.3 + 0.606 * tempC)) / 10000 / 2;

#if DEBUG
  // Debug output
  Serial.print(duration);
  Serial.print("uS - ");
  Serial.print(*dist);
  Serial.println("cm");
#endif // DEBUG

  return dist <= _maxDistance;
}
