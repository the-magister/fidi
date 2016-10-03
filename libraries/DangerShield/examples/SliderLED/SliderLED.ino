#include <DangerShield.h>

const int ledPin = 5;
const int sliderPin = 0;

void setup() {
	// Initialise Danger Shield
	DangerShield.begin();

	// Set led pin output
	pinMode(ledPin, OUTPUT);
}

void loop() {
	// Read slider value from pin A0 (analog) and remap values to 0 to 255 (digital)
	int sliderValue = DangerShield.readSlider(sliderPin, 0, 255);

	// Write the PWM value (0 - 255) to the LED
	// 0 to turn it off, 255 for full brightness
	analogWrite(ledPin, sliderValue);
}
