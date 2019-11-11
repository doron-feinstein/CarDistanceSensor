/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#include "HCSR04.h"

// Timeout for sensor read (equivalentelant to  150ms in microseconds)
#define PULSE_TIMEOUT 150000L

// Maximum measurable distance (400mm according to spec)
#define MAX_DISTANCE 400  

/**
 * Pin for triggering a distance measurement
 */
static int distSensorTrigPin = -1;

/**
 * Pin for the echo return time measurement
 */
static int distSensorEchoPin = -1;

void distSensorInit(int echoPin, int trigPin)
{
  // Initizlize the trigger pint to ouptut and set to low
  distSensorTrigPin = trigPin;
  pinMode(distSensorTrigPin, OUTPUT);
  digitalWrite(distSensorTrigPin, LOW); // Start low
  
  // Initilize the echo pin to input
  distSensorEchoPin = echoPin;
  pinMode(distSensorEchoPin, INPUT);
}

bool measureDist(float tempC, unsigned int* dist)
{
  // Trigger the sensor by sending a 10 microseconds pulse to the sensor
  digitalWrite(distSensorTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(distSensorTrigPin, LOW);

  // Measure the response pulse duration in microseconds
  unsigned long duration = pulseIn(distSensorEchoPin, HIGH, PULSE_TIMEOUT);

  // Duration should never be zero
  if (duration == 0)
  {
    return false;
  }
  
  // Convert duration in microseconds to distance in centimeters
  // Speed of sound is approximately 343 meter per second
  // time needs to be devided by two to account for the time to the object and back
  *dist = (duration * (331.3 + 0.606 * tempC)) / 10000 / 2;

#if DEBUG
  // Debug output
  Serial.print(duration);
  Serial.print("uS - ");
  Serial.print(*dist);
  Serial.println("cm");
#endif // DEBUG

  return (*dist <= MAX_DISTANCE);
}
