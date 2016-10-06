#include <Streaming.h>
#include <Metro.h>
#include <Bounce.h>
#include <DangerShield.h> // see: https://cdn.sparkfun.com/datasheets/Dev/Arduino/Shields/DangerShield_Pin-Outs.pdf
#include <Solenoid.h>

// press the Armed button to unleash fire
#define ARM_BUTTON BUTTON1

#define BEAT_INDICATOR LED1
Bounce Armed = Bounce(ARM_BUTTON, 3UL);

// this is the business end of the affair
#define SOLENOID_PIN LED2 // do not use D9 hereafter
#define SOLENOID_OFF LOW

// hysteresis handling (i.e. "lag")
#define MIN_HYST_TIME 0UL // microseconds
#define MAX_HYST_TIME 1000UL*1000UL // microseconds

Solenoid Fire;

void setup() {
  Serial.begin(115200);

  // Initialise Danger Shield
  DangerShield.begin();

  // Initialize Solenoid
  Fire.begin(SOLENOID_PIN, SOLENOID_OFF);
}

void loop() {

  // Read slider value from "A0/D10" position and map to hysteresis times
  unsigned long laggedValue = map(analogRead(SLIDER1), 0, 1023, MIN_HYST_TIME, MAX_HYST_TIME);

  // Read something from the MIDI side of things and map to total cycle time
  unsigned long cycleTime = 461UL;
  static Metro beatCounter(cycleTime);
  //beatCounter.interval(cycleTime);
  boolean isBeat = beatCounter.check(); // important to keep checking AND CLEARING this.
  
  // Read slider value from "A1/D11" position and map to fraction of the total cycle time
  unsigned long onTime = map(analogRead(SLIDER2), 0, 1023, 0, cycleTime);

  static Metro turnOffBeatLED(10UL);
  if( isBeat ) {
      turnOffBeatLED.interval(onTime);
      turnOffBeatLED.reset();
      digitalWrite( BEAT_INDICATOR, HIGH );
  }
  if( turnOffBeatLED.check() ) {
    digitalWrite( BEAT_INDICATOR, LOW );
  }

  // Update the solenoid timing
  Fire.set( onTime, cycleTime );

  // update the arming system
  if ( Armed.update() ) {
    if ( Armed.read() == LOW ) {

      // show the timings
      Fire.show();
      unsigned long delayFor = cycleTime * 1000UL - laggedValue;
      Serial << F("Lag settings.  lag=") << laggedValue << F(" us. delayFor=") << delayFor << F(" us.") << endl;

      // I think the simplest thing to do is wait here.
      // wait for the next beat
      while ( !beatCounter.check() );
      // wait cycleTime - lag
      delayMicroseconds( delayFor );
      // Arm
      Fire.arm();
      // solenoid will trigger on next .update()
    }
    else {
      Fire.disarm();
    }
  }

  // update the solenoid
  Fire.update();

}
