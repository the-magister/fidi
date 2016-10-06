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
#define MIN_HYST_TIME 0UL // ms
#define MAX_HYST_TIME 1000UL // ms

Solenoid Fire;

class Beats {
  public:
    // set the interval in ms
    void tempo( unsigned long interval );
    // set start of interval
    void start();

    // return TRUE at beat start
    boolean isBeat();
    // return the number of ms until the next beat
    unsigned long nextBeat();
    // return the total cycle time in ms
    unsigned long getTempo();

  private:
    unsigned long interval, lastBeat;

    Metro beatCounter;
};

void Beats::tempo( unsigned long interval ) {
  this->interval = interval;
  this->beatCounter.interval( interval );
}
void Beats::start() {
  this->beatCounter.reset();
  this->lastBeat = millis();
}
boolean Beats::isBeat() {
  if( beatCounter.check() ) {
    this->lastBeat = millis();
    return( true );
  } else {
    return( false );
  }
}
unsigned long Beats::nextBeat() {
  return( this->interval - ( millis() - this->lastBeat ) );
}
unsigned long Beats::getTempo() {
  return( this->interval );
}
Beats Beat;

void setup() {
  Serial.begin(115200);

  // Initialise Danger Shield
  DangerShield.begin();

  // Initialize Solenoid
  Fire.begin(SOLENOID_PIN, SOLENOID_OFF);

  // Initialize Beat
  Beat.tempo( 461UL );
  Beat.start();
}


void loop() {
  // check in with the MIDI signal metronome
  boolean isBeat = Beat.isBeat(); // important to keep checking AND CLEARING this.
  unsigned long cycleTime = Beat.getTempo(); 

  // Read slider value from "A0/D10" position and map to hysteresis times
  unsigned long laggedValue = map(analogRead(SLIDER1), 0, 1023, MIN_HYST_TIME, MAX_HYST_TIME);

  // Read slider value from "A1/D11" position and map to fraction of the total cycle time
  unsigned long onTime = map(analogRead(SLIDER2), 0, 1023, 0, cycleTime);

  // use an LED to track the beat
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
 
      // can we get in on the next beat in time?
      if( Beat.nextBeat() < laggedValue ) {
        // no
        // wait for the next beat
        while ( !Beat.isBeat() );
      }
      // get ready
      delay( Beat.nextBeat() - laggedValue );
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
