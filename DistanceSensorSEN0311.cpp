/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#include <Arduino.h>
#include "DistanceSensorSEN0311.h"

DistanceSensorSEN0311::DistanceSensorSEN0311(unsigned int maxDist)
: _maxDist(maxDist)
, _buffer{0,0,0,0}
, _bufferIdx(0)
, _distanceGood(false)
, _distance(-1)
, _lastReadingTime(millis())
{
}

bool DistanceSensorSEN0311::getReading(unsigned int& dist)
{
  // Update the buffer with the next reading
  if(readPacket())
  {
    // Calculate checksum by adding the header with the two byts of the reading
    int checksum = (_buffer[0]+_buffer[1]+_buffer[2])&0x00FF;
    if(checksum == _buffer[3])
    {
      // Calculate the distance 
      _distance = (_buffer[1]<<8) | _buffer[2]; // Add the bytes to get distance in millimeters

      // Convert to centimeters and handle reading below the measureable limit
      _distance = (_distance > 30) ? _distance / 10 : 0;

      // Clamp the reading to the maximum distance of intrest
      _distance = min(_distance, _maxDist);

      // Mark the distance value as good
      _distanceGood = true;

      /**
       * Update the reading time
       */
      _lastReadingTime = millis();
    }
    else
    {
#if DEBUG
      Serial.println("Checksum failed");
      Serial.print("Header: ");
      Serial.println(_buffer[0], HEX);
      Serial.print("Data high: ");
      Serial.println(_buffer[1], HEX);
      Serial.print("Data low: ");
      Serial.println(_buffer[2], HEX);
      Serial.print("Calculated checksum: ");
      Serial.print(checksum, HEX);
      Serial.print(" expected: ");
      Serial.println(_buffer[3], HEX);
#endif // DEBUG

      // Mark the distance value as bad
      _distanceGood = false;
    }
  }

  // Check if the sensor communication is alive
  if(millis() - _lastReadingTime >= _readingTimeout)
  {
    // The currnet disatance is stale so mark it as bad
    _distanceGood = false;
  }
  
  dist = _distance;
  
  return _distanceGood;
}

void DistanceSensorSEN0311::reset()
{
  // Drop any pending packet data
  _bufferIdx = 0;
  // Turn the last reading to invalid'
  _distanceGood = false;
  
  // Clear any previous pending data
  for(int pendingData = Serial.available(); pendingData >= 0; pendingData--)
  {
    Serial.read();
  }

  // Wait until a full packet is ready to be consumed
  unsigned long waitStartTime = millis();
  while(millis() - waitStartTime <= 800)
  {
    if(Serial.available() > 0)
    {
      if(Serial.peek() != _packetHeader)
      {
        // Align on the header
        Serial.read();
      }
      else if(Serial.available() >= _packetSize)
      {
        // There is a full packet pending so the reset is complete
        break;
      }
    }
    else
    {
      // Wait until more data is avaliable
      delay(10);
    }
  }

#if DEBUG
  Serial.print("Data wait time: ");
  Serial.println(millis() - waitStartTime);
#endif // DEBUG
}

bool DistanceSensorSEN0311::readPacket()
{
  // Read any avaliable data until the end of the packet
  while(_bufferIdx < _packetSize)
  {
    // Make sure there is data to read
    if(Serial.available() == 0)
    {
      return false; // Indicate the packet read is not complete
    }

    // Read the next byte
    _buffer[_bufferIdx] = Serial.read();

    // Make sure the packet is aligned on the header in case the sensor and AVR lost synchronization
    _bufferIdx = _buffer[0] == _packetHeader ? _bufferIdx + 1 : 0;
  }

  // Packet reading is complete
  _bufferIdx = 0;
  return true;
}
