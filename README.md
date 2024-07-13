# Parking Distance Sensor
A small project to help me park my old car
My driveway is tight which makes it challenging to park my old car as close as possible to the end of the driveway.
The car has no distance sensors on the front, so I want to mount a homebrew distance indicator at the end of the driveway.
As the car gets closer to the end of the driveway, the distance indicator will light LED's to indicate the distance. 

## Early Prototype
To get things started I built a prototype on a breadboard, ducktyped it to the end of the driveway and tested it.

### Main Components
The main component is an ATMega328P microcontroller programmed to continuesly measure the car distance and control LEDs to indicate the distance.
Distance measurement uses HC-SR04 ultrasonic range sensor with TMP36 temprature sensor. Since the ultrasonic sensor readings are temprature dependent, the temprature sensor readings are used to compute more accurate distance measurements.
8 LEDs used to indicate how close the car is to the sensor. The conversion between distance measured and the amount of LED's turned on is not linear to allow for more percision close to the sensor.
The prototype could run on 3-4.5v using AA batterys directly connected to the board.

This prototype worked well, but it was not water proof and consumed too much power.
Another issue was that the LED's were not bright enough for day light parking.

