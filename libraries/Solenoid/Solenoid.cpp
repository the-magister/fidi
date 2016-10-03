#include "Solenoid.h"

void Solenoid::setOff() { 
  this->isOn = false;
  digitalWrite(this->pin, this->off);
}
void Solenoid::setOn() { 
  this->isOn = true;
  if( this->isArmed ) digitalWrite(this->pin, this->on);
}
void Solenoid::toggle() { 
  if( this->isOn ) {
    this->setOff();
  } else {
    this->setOn();
  }
}
boolean Solenoid::isFiring() {
  return( isOn );
}

void Solenoid::arm() { // allow firing of the solenoid
  if( !this->isArmed ) {
    Serial << F("**** ARMED ****") << endl;
    this->setOff();
    this->isArmed = true;

    // timer is, by construction, "up"
    this->timer.interval(0);
    this->timer.reset();

  }
}
void Solenoid::disarm() { // disallow firing of the solenoid.
  if( this->isArmed ) {
    Serial << F("** disharmed **") << endl;
    this->isArmed = false;
  }
}

void Solenoid::begin(byte pin, byte offDefinition) {
  this->pin = pin;
  this->off = offDefinition;
  this->on = !offDefinition;
  this->setOff();
  this->disarm();

  // then set the pin to OUTPUT _after_ setting to Off state!
  pinMode(this->pin, OUTPUT);

  Serial << F("Solenoid initializing.  pin=") << this->pin << F(" off=") << this->off << endl;

  this->set(); // start with something.
}

void Solenoid::set(unsigned long onDuration, unsigned long cycleDuration) {
  this->onDuration = onDuration;
  this->offDuration = onDuration<=cycleDuration ? cycleDuration - onDuration : 0UL;
}

void Solenoid::show() {
  Serial << F("Solenoid settings.  pin=") << this->pin << F(". on/off/total=") << this->onDuration << "/";
  Serial << this->offDuration << "/" << this->onDuration+this->offDuration << F(" ms.") << endl; 
}

void Solenoid::update() {

  // is the timer up?
  if( this->timer.check() ) {
    
    // toggle solenoid state
    this->toggle();
    
    if( this->isOn ) {
      // started a cycle
      
      this->timer.interval(this->onDuration);
    } else {
      // ended a cycle
      
      this->timer.interval(this->offDuration);
    }
    this->timer.reset();
  }

}

