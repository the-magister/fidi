#ifndef Solenoid_h
#define Solenoid_h

#include <Arduino.h>

#include <Streaming.h>
#include <Metro.h>

/*
bpm	bps	cycleDuration
60	1	1000
120	2	500
130	2.166666667	461.5384615
180	3	333.3333333
240	4	250
*/

#define FIRE_BALLING_YES_YALLIN 161UL
#define HOUSE_MUSIC_ALL_NIGHT_LONG 461UL

class Solenoid {
  public: 
    // setup
    void begin(byte pin, byte offDefinition=LOW); // set the pin
    void set(unsigned long onDuration=FIRE_BALLING_YES_YALLIN, unsigned long cycleDuration=HOUSE_MUSIC_ALL_NIGHT_LONG); // set timings with immediate effect

    // information
    void show(); // show timings
    
    // operations
    void arm(); // allow firing of the solenoid
    void disarm(); // disallow firing of the solenoid.
    
    void update(); // update the solenoid (ie. run it)
    boolean isFiring(); // returns true if the firing pin is on.

  private:
    byte pin, off, on;

    unsigned long onDuration, offDuration;

    Metro timer;

    boolean isOn, isArmed;

    void setOff();
    void setOn();
    void toggle();
    
};

#endif
