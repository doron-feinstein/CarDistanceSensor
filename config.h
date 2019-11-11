/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#ifndef __CONFIG_H_11_10_2019__
#define __CONFIG_H_11_10_2019__

// Enable serial oupt
#define DEBUG 1

// Temprature sensor analog pin
#define TMP_PIN 0

// Distance sensor digital pins
#define TRIG_PIN 12
#define ECHO_PIN 11

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

#endif // __CONFIG_H_11_10_2019__
