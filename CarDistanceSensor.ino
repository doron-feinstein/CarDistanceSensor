/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#include "config.h"
#include "CarDistanceSensor.h"

CarDistanceSensor carDistanceSensor;

void setup()
{
  // TODO: Serial usage for debug could be cleaned up more
  // RX is used by SEN0311
  // TX can be used for debug messages
  Serial.begin(9600);

  // Initialize
  bool res = carDistanceSensor.init();
#if DEBUG
  if (res)
  {
    Serial.print("Ready");
  }
  else
  {
    Serial.print("initialization failed");
  }
#endif // DEBUG
}

void loop()
{
  carDistanceSensor.update();
}
