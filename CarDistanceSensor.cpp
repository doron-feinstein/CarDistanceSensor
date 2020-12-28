/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#include "config.h"
#include "CarDistanceSensor.h"

CarDistanceSensor::CarDistanceSensor()
: _distanceSensor()
#if USE_SHIFT_REGISTER
, _LEDCtl(8, SR_DATA, SR_CLOCK, SR_LATCH)
#else
, _LEDCtl(8, ledPins)
#endif // USE_SHIFT_REGISTER
, _statusLED(1, (const int[]){10,-1,-1,-1,-1,-1,-1,-1})
{

}

bool CarDistanceSensor::init(bool debug)
{
  bool initSuccess = false;
  // Wait for the sensor to send data
  delay(500);
  // Get one reading to see if the sensor is working
  unsigned int dist = 0;
  initSuccess = _distanceSensor.getReading(dist);

  // Initialize the sleep parameters
  _lastTime = millis();
  _stableDistance = 0;
  _stableDistanceTime = 0;

  return initSuccess;
}

void CarDistanceSensor::update()
{
  bool error = true;
  float tempC = 0.0;
  unsigned int dist = 0;

  error = !_distanceSensor.getReading(dist);
  if (!error)
  {
    // Keep track of the amount of time passed since the distance updated
    unsigned long curTime = millis();

    // Check if the controller time flipped
    if (curTime < _lastTime)
    {
      _lastTime = 0;
    }

    // Update the amount of time the distance was stable
    if (abs((int)_stableDistance - (int)dist) <= SLEEP_DIST_THESHOLD)
    {
      // Distance is stable - keep counting the time
      _stableDistanceTime += curTime - _lastTime;
    }
    else
    {
      // Distance is not stable - reset the distance and time
      _stableDistance = dist;
      _stableDistanceTime = 0;
    }

    // Check if distance was stable for long enough
    if (_stableDistanceTime >= SLEEP_DELAY_TIME)
    {
      if (_stableDistanceTime <= (SLEEP_FLASH_TIME + SLEEP_DELAY_TIME))
      {
        // Flash the LEDs before turning off the light
        _LEDCtl.flashLights();

        // Indicate status is good
        _statusLED.updateLights(1);
      }
      else
      {
        // Turn off all LEDs
        _LEDCtl.updateLights(0);
      }
    }
    else
    {
      // Update the dispaly based on the current distance
      updateDisplay(dist);

      // Indicate status is good
      _statusLED.updateLights(1);
    }

#if DEBUG // Debug output
    Serial.print("Last Dist: ");
    Serial.println(_stableDistance);
    Serial.print(" last time: ");
    Serial.println(_stableDistanceTime);
#endif // DEBUG

    // Update last time
    _lastTime = curTime;
  }
  else
  {
    #if DEBUG // Debug output
      Serial.println("Error in distance measurement!");
    #endif // DEBUG

    // Indicate error
    _statusLED.flashLights();

    // Wait to make sure everyting has sattled
    delay(10);
  }
}

void CarDistanceSensor::updateDisplay(unsigned int dist)
{
  if(dist <= 3)
  {
    // Flash the LEDs to indicate the car is too close
    _LEDCtl.flashLights();
  }
  else
  {
    // Turn on the LEDs based on the distance thresholds
    byte ligthtFlags = 0;
    for (int i = 0; i < 8; i++)
    {
      ligthtFlags |= ((dist < threshold[i] ? 1 : 0) << i);
    }
    _LEDCtl.updateLights(ligthtFlags);
  }
}
