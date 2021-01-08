/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#include "config.h"
#include "CarDistanceSensor.h"

// Includes needed for setting the ATMega to sleep and wake up by the watchdog
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

CarDistanceSensor::CarDistanceSensor()
: _distanceSensor(threshold[0] + 5)
#if USE_SHIFT_REGISTER
, _LEDCtl(8, SR_DATA, SR_CLOCK, SR_LATCH)
#else
, _LEDCtl(8, ledPins)
#endif // USE_SHIFT_REGISTER
, _statusLED(1, (const int[]){10,-1,-1,-1,-1,-1,-1,-1})
, _lastTime(0)
, _stableDistance(0)
, _stableDistanceTime(0)
, _errorStartTime(0)
{
  // Turn on the distance sensor
  pinMode(DS_POWER, OUTPUT);
  digitalWrite(DS_POWER, HIGH);
}

bool CarDistanceSensor::init()
{
  // Disable unneeded features to save some power
  initPowerSaving();

  bool initSuccess = false;

  // Reset the communication with the sensor
  _distanceSensor.reset();
  // Get one reading to see if the sensor is working
  unsigned int dist = 0;
  initSuccess = _distanceSensor.getReading(dist);

  // Initialize the LED flash / sleep parameters
  _lastTime = millis();
  _stableDistance = 0;
  _stableDistanceTime = 0;

  return initSuccess;
}

// ATMega reset function
void(* resetFunc) (void) = 0;

void CarDistanceSensor::update()
{
  bool error = true;
  unsigned int dist = 0;

  // Keep track of the amount of time passed since the distance updated
  unsigned long curTime = millis();

  // Check if the controller time flipped
  if (curTime < _lastTime)
  {
    _lastTime = 0;
  }

  // Get a reading from the sensor
  error = !_distanceSensor.getReading(dist);
  if (!error)
  {
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
        // Go to sleep until the distance reading changes
        sendToSleep();
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
    Serial.print(_stableDistance);
    Serial.print(" last time: ");
    Serial.println(_stableDistanceTime);
#endif // DEBUG

    // Update last time
    _lastTime = curTime;

    // Indicate no error
    _errorStartTime = 0;
  }
  else
  {
#if DEBUG // Debug output
    Serial.println("Error in distance measurement!");
#endif // DEBUG

    // Update the error start time if needed
    if(_errorStartTime == 0)
    {
      _errorStartTime = curTime;
    }

    // Check if its time to reset the device
    // TODO: Handle timer flip
    if((curTime - _errorStartTime) >= ERROR_RECOVERY_TIME)
    {
      // Power down the sensor enough to reset it
      digitalWrite(DS_POWER, LOW);
      delay(50);

#if DEBUG // Debug output
      Serial.println("Reseting board");
      Serial.flush();
#endif // DEBUG

      // Restart the ATMega
      resetFunc();  //call reset
    }

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

void CarDistanceSensor::initPowerSaving()
{
  // Disable SPI
  SPCR = 0;
  power_spi_disable();

  // Disable I2C
  TWCR = 0;
  power_twi_disable();

  // Disable ADC
  ADCSRA = 0;
  power_adc_disable();
}

void CarDistanceSensor::sendToSleep()
{
#if DEBUG
  unsigned int count = 1;
  Serial.println("Entring sleep mode");
  Serial.flush();
#endif // DEBUG

  unsigned int dist = 0;
  do
  {
    // Turn off all LEDs
    _LEDCtl.updateLights(0);
    _statusLED.updateLights(0);

    // Set power down sleep mode for maximum savings
    set_sleep_mode (SLEEP_MODE_PWR_DOWN);
    // Enable sleep mode
    sleep_enable();

    // Disable low voltage detection during sleep
    sleep_bod_disable();

    // Disable interrupts while setting up the watchdog
    noInterrupts();
    // Clear all reset flags
    MCUSR = 0;
    // Enable watchdog changes and disable reset
    WDTCSR = bit(WDCE) | bit(WDE);
    // Set watchdog to interrupt mode with an interval to 1 second
    WDTCSR = bit(WDIE) | bit(WDP2) | bit(WDP1);
    // Start over the watchdog count
    wdt_reset();
    // Watch dog is ready so interrupts can be enabled
    interrupts();

    // Go to sleep
    sleep_cpu();

    // Done sleeping for now (this should be called after the watchdog triggered and woke the ATMega)
    sleep_disable();

    // Reset the packet in case it contains a partial read
    _distanceSensor.reset();

    // Turn on status light and leave it on for a little while in case its not time to wake up
    _statusLED.updateLights(1);
    delay(5);

#if DEBUG
    Serial.print("Upate ");
    Serial.print(count++);
    Serial.print(" dist ");
    Serial.print(dist);
    Serial.print(" stable dist ");
    Serial.println(_stableDistance);
    Serial.flush();
#endif // DEBUG
  } while(_distanceSensor.getReading(dist) && (abs((int)_stableDistance - (int)dist) <= SLEEP_DIST_THESHOLD));

#if DEBUG
  Serial.println("Leaving sleep mode");
#endif // DEBUG
}

ISR(WDT_vect)
{
  // Disable the watchdog without resetting and go back go the main code
  wdt_disable();
}
