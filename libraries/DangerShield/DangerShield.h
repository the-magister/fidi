/*
 Danger Shield library for DangerShield v1.7
 Written by Maarten de Boer 10-3-2014

 Original code written by Chris Taylor 3-3-2010
 Original code Tweaked by Nathan Seidle 10-22-2012

 Forked from: https://github.com/sparkfun/DangerShield

 The code is licensed public domain but buy the guys from SparkFun a beer when you meet them some day! (Beerware license)

 The idea of a Danger Shield was originally created by Zach Hoeken (http://www.zachhoeken.com/) released CC-SA.
*/

#ifndef DangerShield_h
#define DangerShield_h

#include "Arduino.h"

// Analog
#define SLIDER1  A2 //Matches button 1
#define SLIDER2  A1
#define SLIDER3  A0 //Matches button 3

#define LIGHT    A3
#define TEMP     A4

// Digital
#define BUZZER   3
#define DATA     4

#define LED1     5
#define LED2     6

#define LATCH    7
#define CLOCK    8

#define BUTTON1  10
#define BUTTON2  11
#define BUTTON3  12

// This version has a capacitive sensor which relies on the Capactive Sensor library
// which can be found here: http://playground.arduino.cc/Main/CapacitiveSensor

// 7-segment display
const byte ledCharSet[11] = {
	B00111111,
	B00000110,
	B01011011,
	B01001111,
	B01100110,
	B01101101,
	B01111101,
	B00000111,
	B01111111,
	B01101111,
	B10000000
};

class DangerShieldClass {
	public:
		// Initialisation
		void begin();

		// Temperature
		float readTemperature();

		// Remapped light sensor
		long readLight(int min, int max);

		// Remapped slider
		long readSlider(int pin, int min, int max);

		// 7-segment display
		void setSegmentDisplay(char value);
		void setSegmentDisplay(int value);
		void clearSegmentDisplay();
};

extern DangerShieldClass DangerShield;

#endif
