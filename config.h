/**
 * Car distance sensor project by Doron Feinstein
 * @see https://github.com/doron-feinstein/CarDistanceSensor.git
 */

#ifndef __CONFIG_H_11_10_2019__
#define __CONFIG_H_11_10_2019__

// Enable serial oupt
#define DEBUG 0

// Power pin for the distance sensor
#define DS_POWER 11

// Amount of time allowed for recovery from error in milliseconds
#define ERROR_RECOVERY_TIME 500

// We can save a few ATMega pins by using a shift register for the LEDs
// At the moment this is not needed as this project doesnt use all the pins
#define USE_SHIFT_REGISTER 0
#if USE_SHIFT_REGISTER
// Shift register pins
#define SR_DATA 8
#define SR_CLOCK 10
#define SR_LATCH 9
#else
// When not using the shift register the LED pins need to be defined
// The array goes from the green LED for long distance at index 0 to
//  the red LED at the shortest distance
static int ledPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
#endif // USE_SHIFT_REGISTER

// Amount of time in ms to wait before shutting down in milliseconds
#define SLEEP_DELAY_TIME 10000
#define SLEEP_FLASH_TIME 2000
#define SLEEP_DIST_THESHOLD 5

// LED distance thresholds in cm
const unsigned int threshold[8] = {80,70,60,50,40,30,20,10};

#endif // __CONFIG_H_11_10_2019__
