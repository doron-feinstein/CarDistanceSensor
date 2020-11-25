/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#include "config.h"
#include "CarDistanceSensor.h"

CarDistanceSensor carDistanceSensor;

void setup()
{
  bool debug = false;
#if DEBUG
  Serial.begin(9600);
  debug = true;
#endif // DEBUG

  bool res = carDistanceSensor.init(debug);
  if(debug)
  {
    if (res)
    {
      Serial.print("Ready");
    }
    else
    {
      Serial.print("initialization failed");
    }
  }
}

void loop()
{
  carDistanceSensor.update();
}
