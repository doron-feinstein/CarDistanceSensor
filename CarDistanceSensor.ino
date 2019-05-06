// Enable serial oupt
#define DEBUG 1

// TMP36 sensor analog pin
#define TEMP_PIN 0

// Echo sensor 
#define TRIG_PIN 12
#define ECHO_PIN 11
#define PULSE_TIMEOUT 150000L  // 150ms in microseconds
#define MAX_DISTANCE 400  // According to spec can measure up to 4 meter (400mm)

// We can save a few ATMega pins by using a shift register for the LEDs
// At the moment this is not needed as this project doesnt use all the pins
#define USE_SHIFT_REGISTER 0
#if USE_SHIFT_REGISTER
// Shift register pins
#define DATA 8
#define CLOCCK 10
#define LATCH 9
#else
// When not using the shift register the LED pins need to be defined
// The array goes from the green LED for long distance at index 0 to
//  the red LED at the shortest distance
int ledPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
#endif // USE_SHIFT_REGISTER

// Amount of time in ms to wait before shutting down
#define SLEEP_DELAY 10000
#define SLEEP_DIST_THESHOLD 5

// LED distance thresholds in cm
long threshold[8] = {80,70,60,50,40,30,20,10};

// Last measured time
unsigned long lastTime;
unsigned int lastDistance;
unsigned long lastStableDistanceTime;
unsigned long lastFlashTime;

void setup()
{
  // Setup the sensor
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW); // Start low

#if USE_SHIFT_REGISTER
  // Setup the shift register
  pinMode(DATA, OUTPUT);
  pinMode(CLOCCK, OUTPUT);
  digitalWrite(CLOCCK, LOW);
  pinMode(LATCH, OUTPUT);
  digitalWrite(LATCH, LOW);
#else
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

#if DEBUG
  Serial.begin(9600);
#endif // DEBUG
}

void loop()
{
  bool error = true;
  float tempC = 0.0;
  unsigned int dist = MAX_DISTANCE;
  
  // Get the current temprature measurement
  if (measureTemp(&tempC))
  {
    // Get the current distance measurement
    if (measureDist(tempC, &dist))
    {
      // Indicate success
      error = false;
    }
  }

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

bool measureTemp(float* measuredTempC)
{
  // Get the reading from the pin
  int tempReading = analogRead(TEMP_PIN);

  // Convert to volts
  float tempVolts = tempReading * 5.0f / 1024.0f;

  // Convert volts to temprature in celcius
  float tempC = (tempVolts - 0.5) * 10.0;

#if DEBUG
  // Debug output
  Serial.print(tempC);
  Serial.println("C");
#endif // DEBUG

  // Indicate an error when the temprature is out of range
  // TODO: Replace with actual sensor spec
  if (tempC < -40 || tempC > 60)
  {
    return false;
  }

  // Store the measured temprature
  *measuredTempC = tempC;

  return true;
}

bool measureDist(float tempC, unsigned int* dist)
{
  // Trigger the sensor
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the response pulse duration in microseconds
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, PULSE_TIMEOUT);

  // Duration should never be zero
  if (duration == 0)
  {
    return false;
  }
  
  // Convert duration in microseconds to distance in centimeters
  // Speed of sound is approximately 343 meter per second
  // time needs to be devided by two to account for the time to the object and back
  *dist = (duration * (331.3 + 0.606 * tempC)) / 10000 / 2;

  if (*dist > MAX_DISTANCE)
  {
    return false;
  }

#if DEBUG
  // Debug output
  Serial.print(duration);
  Serial.print("uS - ");
  Serial.print(*dist);
  Serial.println("cm");
#endif // DEBUG

  return true;
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
