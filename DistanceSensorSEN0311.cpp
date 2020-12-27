/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#include <Arduino.h>
#include "DistanceSensorSEN0311.h"

DistanceSensorSEN0311::DistanceSensorSEN0311(int txPin, int rxPin)
: _SerialPort(rxPin, txPin)
, _buffer{0,0,0,0}
, _bufferIdx(0)
, _distanceGood(false)
, _distance(-1)
, _lastReadingTime(millis())
{
  // Open the communication channel
  _SerialPort.begin(9600);
}

bool DistanceSensorSEN0311::getReading(unsigned int& dist)
{
  // Update the buffer with the next reading
  if(readPacket())
  {
    // Calculate checksum by adding the header with the two byts of the reading
    int checksum = (_buffer[0]+_buffer[1]+_buffer[2])&0x00FF;
    if(_buffer[0] == 0xFF && checksum == _buffer[3])
    {
      // Calculate the distance 
      _distance = (_buffer[1]<<8)+_buffer[2];
      if(_distance > 30)
      {
        _distance = _distance / 10;
      }
      else 
      {
        // Indiate the object is too close
        _distance = 0;
      }

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

bool DistanceSensorSEN0311::readPacket()
{
  // Read any avaliable data until the end of the packet
  while(_bufferIdx < 4)
  {
    // Make sure there is data to read
    if(_SerialPort.available() == 0)
    {
      return false; // Indicate the packet read is not complete
    }
    _buffer[_bufferIdx] = _SerialPort.read();

    // Make sure the packet is aligned on the header in case the sensor and AVR lost synchronization
    _bufferIdx = _buffer[0] == 0xFF ? _bufferIdx + 1 : 0;
  }

  // Packet reading is complete
  _bufferIdx = 0;
  return true;
}
