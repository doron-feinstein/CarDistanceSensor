/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#ifndef __DISTANCESENSOR_SEN0311_H_11_25_2020__
#define __DISTANCESENSOR_SEN0311_H_11_25_2020__

#include <SoftwareSerial.h>

/**
 * SEN0311 is a waterproof ultrasonic distance sensor
 * Values are obtained through serial communication at 9600bit/s baud rate
 * 
 */
class DistanceSensorSEN0311
{
public:

  /**
   * Construct with given communication pins
   * @param Transmit pin
   * @param Receive pin
   */
  DistanceSensorSEN0311(int txPin, int rxPin);

  /**
   * Get a measurement reading from the sensor
   * This function tries to read a new distance value, but returns the old one if no new value is avaliable
   * If the object is too close to be sensed the distance is set to 0
   * @param Reference for the distance measured in centimeters
   * @return True if measurement was taken successfully, otherwise false
   */
  bool getReading(unsigned int& dist);

private:

  /**
   * Read single packet data into the buffer as it becomes avaliable
   * @return True if a new packet has been fully read, otherwise false
   */
  bool readPacket();
  
  /**
   * Transmit pin
   */
  int _txPin;
  
  /**
   * Receive pin
   */
  int _rxPin;

  /**
   * Serial port for communication with the sensor
   */
  SoftwareSerial _SerialPort;

  /**
   * Communication buffer which stores the following:
   * Header as a byte
   * Distance reading as a short
   * Checksum as a byte
   */
   unsigned char _buffer[4];

  /**
   * Current buffer read index
   */
  int _bufferIdx;

  /**
   * Is the last reading good
   * The reading is considered good if a full packet was read with the expected header and checksum
   */
  bool _distanceGood;
  
  /**
   * Last good reading
   */
  unsigned int _distance;
};

#endif // __DISTANCESENSOR_SEN0311_H_11_25_2020__
