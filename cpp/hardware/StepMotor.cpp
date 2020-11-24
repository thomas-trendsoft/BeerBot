#include <wiringPi.h>
#include "StepMotor.h"

// default constructor
StepMotor::StepMotor() {
  // setup gpio pins
  pinMode(STEPPIN0,OUTPUT);
  pinMode(STEPPIN1,OUTPUT);
  pinMode(STEPPIN2,OUTPUT);
  pinMode(STEPPIN3,OUTPUT);
}

// rotate motor
void StepMotor::rotate(int steps,int dir) {

  // iterate steps
  for (int s=0;s<steps;s++) {
    // iterate pins
    for (int ps=0;ps<4;ps++) {
      int pin = this->pins[ps];
      int val = this->sequence[seqidx][ps];
      digitalWrite(pin,val);
      // update offset
      this->seqidx += dir;
      // check borders
      if (this->seqidx < 0) this->seqidx = 4 - 1;
      if (this->seqidx > 3) this->seqidx = 0;
      // wait for step
      delay(30);
    }
  }

}
