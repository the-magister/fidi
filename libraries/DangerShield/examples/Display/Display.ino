#include <DangerShield.h>

const int sliderPin = 0;

void setup() {
	// Initialise Danger Shield
	DangerShield.begin();
}

void loop() {
	// Read slider value from pin A0 (analog) and remap values to 0 to 9
	int sliderValue = DangerShield.readSlider(sliderPin, 0, 9);

	// Set 7-segment display value
	DangerShield.setSegmentDisplay(sliderValue);
}
