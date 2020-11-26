/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#include "config.h"
#include "CarDistanceSensor.h"

CarDistanceSensor::CarDistanceSensor()
#if USE_DIST_SENS_SEN0311
: _distanceSensor(TX_PIN, RX_PIN)
#else
: _distanceSensor(TRIG_PIN, ECHO_PIN)
#endif // USE_DIST_SENS_SEN0311
{
  
}

bool CarDistanceSensor::init(bool debug)
{
  bool initSuccess = false;
#if USE_DIST_SENS_SEN0311
  // Wait for the sensor to send data
  delay(500);
  // Get one reading to see if the sensor is working
  unsigned int dist = 0;
  initSuccess = _distanceSensor.getReading(dist);
#else
  // Setup the temprature sensor
  initSuccess = tmpSensorInit(TMP_PIN);
  if(debug)
  {
    if (tmpReady)
    {
      Serial.println("Temprature sensor ready");
    }
    else
    {
      Serial.println("Temprature sensor failed to initialize");
    }
  }
#endif // USE_DIST_SENS_SEN0311

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

#if USE_DIST_SENS_SEN0311
  error = !_distanceSensor.getReading(dist);
#else
  // Get the current temprature measurement
  if (measureTmpC(&tempC))
  {
#if DEBUG
  // Debug output
  Serial.print(tempC);
  Serial.println("C");
#endif // DEBUG

    // Get the current distance measurement
    if(_distanceSensor.getReading(tempC, dist))
    {
      // Indicate success
      error = false;
    }
  }
#endif // USE_DIST_SENS_SEN0311

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
      }
      else
      {
        // Turn off all LEDs
        _LEDCtl.writeByte(0);
      }
    }
    else
    {
      updateDisplay(dist);
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
    _LEDCtl.writeByte(ligthtFlags);
  }
}
