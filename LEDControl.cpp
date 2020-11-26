/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */
 
#include "LEDControl.h"

#include "config.h"

LEDControl::LEDControl()
: _lastUpdateTime(0)
{
#if USE_SHIFT_REGISTER
  // Setup the shift register
  pinMode(DATA, OUTPUT);
  pinMode(CLOCCK, OUTPUT);
  digitalWrite(CLOCCK, LOW);
  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, LOW);
#else
  // Dedicate a pin for each LED
  for (int i = 0; i < 8; i++)
  {
    pinMode(ledPins[i], OUTPUT);
  }
#endif // USE_SHIFT_REGISTER
}

void LEDControl::flashLights()
{
  byte ligthtFlags = (_lastUpdateTime % 333 > 100) ? 0 : 0xFF;
  writeByte(ligthtFlags);
}

void LEDControl::writeByte(byte b)
{
#if USE_SHIFT_REGISTER
  // Disable shift register update until the entire data is written
  digitalWrite(LATCH, LOW);
  
  // Write the data to the shift register
  shiftOut(DATA, CLOCCK, MSBFIRST, b);

  // Update the shift register output
  digitalWrite(LATCH, HIGH);
#else
  //Update the LED pins directly
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(ledPins[i], (b & (1 << i)) != 0 ? HIGH : LOW);
  }
#endif // USE_SHIFT_REGISTER

  // Update the internal time
  _lastUpdateTime = millis();
}
