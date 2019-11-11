/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#include "config.h"
#include "TMP36.h"
#include "HCSR04.h"

// Last measured time
unsigned long lastTime;
unsigned int lastDistance;
unsigned long lastStableDistanceTime;
unsigned long lastFlashTime;

void setup()
{
#if DEBUG
  Serial.begin(9600);
#endif // DEBUG

  // Setup the temprature sensor
  bool tmpReady = tmpSensorInit(TMP_PIN);
#if DEBUG
  if (tmpReady)
  {
    Serial.print("Temprature sensor ready");
  }
  else
  {
    Serial.print("Temprature sensor failed to initialize");
  }
#endif // DEBUG

  // Setup the distance sensor
  distSensorInit(ECHO_PIN, TRIG_PIN);

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

  // Initialize the sleep parameters
  lastTime = millis();
  lastDistance = 0;
  lastStableDistanceTime = 0;
  lastFlashTime = 0;
}

void loop()
{
  bool error = true;
  float tempC = 0.0;
  unsigned int dist = 0;
  
  // Get the current temprature measurement
  if (measureTmpC(&tempC))
  {
    // Get the current distance measurement
    if (measureDist(tempC, &dist))
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
    lastDistance = dist;//(unsigned int)((float)lastDistance * 0.9f + (float)dist * 0.1f);

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

  // Wait before measuring again
  //delay(50);
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

void writeByte(byte b)
{
#if DEBUG
  Serial.print("writing: ");
  Serial.println(b);
#endif // DEBUG

#if USE_SHIFT_REGISTER
  // Disable shift register update until the entire data is written
  digitalWrite(LATCH, LOW);
  
  // Write the data to the shift register
  shiftOut(DATA, CLOCCK, MSBFIRST, b);

  // Update the shift register output
  digitalWrite(LATCH, HIGH);
#else
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(ledPins[i], (b & (1 << i)) != 0 ? HIGH : LOW);
  }
#endif // USE_SHIFT_REGISTER
}
