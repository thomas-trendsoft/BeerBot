#include <wiringPi.h>
#include <iostream>
#include "StepMotor.h"

// default constructor
StepMotor::StepMotor() {
  this->seqidx = 0;
  // setup gpio pins
  pinMode(STEPPIN0,OUTPUT);
  pinMode(STEPPIN1,OUTPUT);
  pinMode(STEPPIN2,OUTPUT);
  pinMode(STEPPIN3,OUTPUT);
}

void StepMotor::pause() {
  digitalWrite(STEPPIN0,LOW);
  digitalWrite(STEPPIN1,LOW);
  digitalWrite(STEPPIN2,LOW);
  digitalWrite(STEPPIN3,LOW);
}

// rotate motor
void StepMotor::rotate(int steps,int dir) {

  // iterate steps
  for (int s=0;s<steps;s++) {
    // iterate pins
    for (int ps=0;ps<4;ps++) {
      int pin = this->pins[ps];
      int val = this->sequence[this->seqidx][ps];
      digitalWrite(pin,val);
    }
    // update offset
    this->seqidx += dir;
    // check borders
    if (this->seqidx < 0) this->seqidx = 4 - 1;
    if (this->seqidx > 3) this->seqidx = 0;
    // wait for step
    delay(5);
  }

}
