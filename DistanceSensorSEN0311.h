/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#ifndef __DISTANCESENSOR_SEN0311_H_11_25_2020__
#define __DISTANCESENSOR_SEN0311_H_11_25_2020__

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
   * Use maxDist to clamp the reading to a given maximum distance which helps avoid noise in distance spectrum we dont care about
   * @param Maximum distance of intrest
   */
  DistanceSensorSEN0311(unsigned int maxDist);

  /**
   * Get a measurement reading from the sensor
   * This function tries to read a new distance value, but returns the old one if no new value is avaliable
   * If the object is too close to be sensed the distance is set to 0
   * @param Reference for the distance measured in centimeters
   * @return True if measurement was taken successfully, otherwise false
   */
  bool getReading(unsigned int& dist);

  /**
   * Reset the communication with the sensor
   * If the communication is in order a new packet should be ready for consumption
   */
  void reset();

private:

  /**
   * Read single packet data into the buffer as it becomes avaliable
   * @return True if a new packet has been fully read, otherwise false
   */
  bool readPacket();

private:

  /**
   * Maximum distance of intrest which we dont care about values further away
   * Any reading past this value is clamped to this value
   */
  unsigned int _maxDist;

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

  /**
   * Last time the sensor returned a good reading
   */
  unsigned long _lastReadingTime;

  /**
   * Based on the spec the response time should be 100-300ms in processed mode, so the timeout adds some margin for error
   */
  const unsigned long _readingTimeout = 400L;

  /**
   * Expected packet header value
   */
  const unsigned char _packetHeader = 0xFF;

  /**
   * Number of bytes in one packet
   */
  const int _packetSize = 4;
};

#endif // __DISTANCESENSOR_SEN0311_H_11_25_2020__
