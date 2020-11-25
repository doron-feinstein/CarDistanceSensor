/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#include "config.h"
#include "CarDistanceSensor.h"

#include "TMP36.h"
#include "LEDControl.h"

// Last measured time
unsigned long lastTime;
unsigned int lastDistance;
unsigned long lastStableDistanceTime;
unsigned long lastFlashTime;

CarDistanceSensor::CarDistanceSensor()
: _distanceSensor(TRIG_PIN, ECHO_PIN)
{
  
}

bool CarDistanceSensor::init(bool debug)
{
  // Setup the temprature sensor
  bool tmpReady = tmpSensorInit(TMP_PIN);
  if(debug)
  {
    if (tmpReady)
    {
      Serial.print("Temprature sensor ready");
    }
    else
    {
      Serial.print("Temprature sensor failed to initialize");
    }
  }

  // Initialize the LED control
  initLEDCtl();

  // Initialize the sleep parameters
  lastTime = millis();
  lastDistance = 0;
  lastStableDistanceTime = 0;
  lastFlashTime = 0;

  return tmpReady;
}

void updateLights(long dist)
{
  byte ligthtFlags = 0;
  for (int i = 0; i < 8; i++)
  {
    ligthtFlags |= ((dist < threshold[i] ? 1 : 0) << i);
  }
  writeByte(ligthtFlags);
}

void flashLights()
{
  byte ligthtFlags = (lastFlashTime % 333 > 100) ? 0 : 0xFF;
  writeByte(ligthtFlags);
}

void CarDistanceSensor::update()
{
  bool error = true;
  float tempC = 0.0;
  unsigned int dist = 0;
  
  // Get the current temprature measurement
  if (measureTmpC(&tempC))
  {
    // Get the current distance measurement
    if(_distanceSensor.getReading(tempC, dist))
    {
      // Indicate success
      error = false;
    }
  }

#if DEBUG
  // Debug output
  Serial.print(tempC);
  Serial.println("C");
#endif // DEBUG

  if (!error)
  {
    // Keep track of the amount of time passed since the distance updated
    unsigned long curTime = millis();

    // Check if the controller time flipped
    if (curTime < lastTime)
    {
      lastTime = 0;
    }

    // Update the amount of time the distance was stable
    if (abs((int)lastDistance - (int)dist) <= SLEEP_DIST_THESHOLD)
    {
      // Distance is stable - keep counting the time
      lastStableDistanceTime += curTime - lastTime;
    }
    else
    {
      // Distance is not stable - reset the time
      lastStableDistanceTime = 0;
      lastFlashTime = 0;
    }

    // Check if distance was stable for long enough
    if (lastStableDistanceTime >= SLEEP_DELAY)
    {
      if (lastFlashTime <= 1000)
      {
        flashLights();

        // Keep counting the time
        lastFlashTime += curTime - lastTime;
      }
      else
      {
        writeByte(0);
      }
    }
    else
    {      
      updateLights(dist);
    }

    // Update the last distance
    lastDistance = dist;

#if DEBUG // Debug output
    Serial.print("Last Dist: ");
    Serial.print(lastDistance);
    Serial.print(" last time: ");
    Serial.println(lastStableDistanceTime);
#endif // DEBUG

    // Update last time
    lastTime = curTime;
  }
  else
  {
    #if DEBUG // Debug output
      Serial.println("Error in distance measurement!");
    #endif // DEBUG

    // Wait to make sure everyting has sattled
    delay(10);
  }
}
