/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */
 
#include "LEDControl.h"

#include "config.h"

LEDControl::LEDControl(int numLights, int dataPin, int clockPin, int latchPin)
: _numLights(numLights)
, _usingShiftRegister(true)
, _dataPin(dataPin)
, _clockPin(clockPin)
, _latchPin(latchPin)
, _lastUpdateTime(0)
{
  // Setup the shift register
  pinMode(_dataPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);
  digitalWrite(_clockPin, LOW);
  pinMode(_latchPin, OUTPUT);
  digitalWrite(_latchPin, LOW);

  // Turn off all the lights
  updateLights(0);
}

LEDControl::LEDControl(int numLights, const int ledPins[])
: _numLights(numLights)
, _usingShiftRegister(false)
, _lastUpdateTime(0)
{
  // Dedicate a pin for each LED
  for (int i = 0; i < numLights; i++)
  {
    _ledPins[i] = ledPins[i];
    pinMode(_ledPins[i], OUTPUT);
  }

  // Turn off all the lights
  updateLights(0);
}

void LEDControl::flashLights()
{
  byte ligthtFlags = (_lastUpdateTime % 333 > 100) ? 0 : 0xFF;
  updateLights(ligthtFlags);
}

void LEDControl::updateLights(byte b)
{
  if(_usingShiftRegister)
  {
    // Disable shift register update until the entire data is written
    digitalWrite(_latchPin, LOW);
  
    // Write the data to the shift register
    shiftOut(_dataPin, _clockPin, MSBFIRST, b);

    // Update the shift register output
    digitalWrite(_latchPin, HIGH);
  }
  else
  {
    //Update the LED pins directly
    for (int i = 0; i < _numLights; i++)
    {
      digitalWrite(_ledPins[i], (b & (1 << i)) != 0 ? HIGH : LOW);
    }
  }

  // Update the internal time
  _lastUpdateTime = millis();
}
