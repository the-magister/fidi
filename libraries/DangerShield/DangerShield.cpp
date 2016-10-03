/*
 Danger Shield library for DangerShield v1.7
 Written by Maarten de Boer 10-3-2014

 Original code written by Chris Taylor 3-3-2010
 Original code Tweaked by Nathan Seidle 10-22-2012

 Forked from: https://github.com/sparkfun/DangerShield

 The code is licensed public domain but buy the guys from SparkFun a beer when you meet them some day! (Beerware license)

 The idea of a Danger Shield was originally created by Zach Hoeken (http://www.zachhoeken.com/) released CC-SA.
*/

#include "DangerShield.h"

/**
 * begin
 *
 * Initialisation function to set correct pin mode on all digital in-/outputs
*/
void DangerShieldClass::begin() {
	// Inputs
  pinMode(SLIDER1, INPUT);
  pinMode(SLIDER2, INPUT);
  pinMode(SLIDER3, INPUT);
  pinMode(LIGHT, INPUT);
  pinMode(TEMP, INPUT);

  //Enable internal pullups
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);

	// Outputs
  pinMode(BUZZER, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  pinMode(LATCH, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(DATA, OUTPUT);
}

/**
 * readTemperature
 *
 * Read temperature in degrees celcius from on-board temperature meter
 * connected to analog port A4
 *
 * @returns Measured temperature in degrees celcius
*/
float DangerShieldClass::readTemperature() {
	long temp = analogRead(TEMP);

	temp *= 5000;
	temp /= 1024;
	temp -= 500;

	return temp / 10;
}

/**
 * readLight
 *
 * Read the value from the analog LDR (light sensor) and return its value
 * remapped to the specified min and max value.
 *
 * @returns long Relative intensity of measured light, remapped to the specified min and max
*/
long DangerShieldClass::readLight(int min, int max) {
	return map(analogRead(LIGHT), 0, 1023, min, max);
}

/**
 * readSlider
 *
 * Read the value from an analog slider and return its value
 * remapped to the specified min and max value.
 *
 * @returns long Value of the specified slider, remapped to the specified min and max
*/
long DangerShieldClass::readSlider(int pin, int min, int max) {
	return map(analogRead(pin), 0, 1023, min, max);
}

/**
 * setSegementDisplay
 *
 * Set the 7-segment display to the specified value [0 - 9] or '.'
 * char value Value between 0 and 9 or '.' to display on the 7-segment display
 *
*/
void DangerShieldClass::setSegmentDisplay(char value) {
	if(value != '.' && (value < '0' || value > '9'))
		return;

	byte ledChar;

	if(value == '.') {
		ledChar = ledCharSet[10];
	} else {
		ledChar = ledCharSet[value - 48];
	}

	digitalWrite(LATCH, LOW);
	shiftOut(DATA, CLOCK, MSBFIRST, ~ledChar);
	digitalWrite(LATCH, HIGH);
}

/**
 * setSegementDisplay
 *
 * Set the 7-segment display to the specified value [0 - 9]
 * int value Value between 0 and 9 to display on the 7-segment display
 *
*/
void DangerShieldClass::setSegmentDisplay(int value) {
	if(value < 0 || value > 9) {
		return;
	}

	digitalWrite(LATCH, LOW);
	shiftOut(DATA, CLOCK, MSBFIRST, ~ledCharSet[value]);
	digitalWrite(LATCH, HIGH);
}

/**
 * clearSegementDisplay
 *
 * Clear the 7-segment display
 *
*/
void DangerShieldClass::clearSegmentDisplay() {
	digitalWrite(LATCH, LOW);
	shiftOut(DATA, CLOCK, MSBFIRST, B11111111);
	digitalWrite(LATCH, HIGH);
}

DangerShieldClass DangerShield;
