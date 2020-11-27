#include <wiringPi.h>
#include <softPwm.h>
#include "DriveMotor.h"

//
// default constructor
//
DriveMotor::DriveMotor() {

  // initialize gpio pins
  pinMode(DCM_IN1,OUTPUT);
  pinMode(DCM_IN2,OUTPUT);
  pinMode(DCM_IN3,OUTPUT);
  pinMode(DCM_IN4,OUTPUT);
  pinMode(DCM_EN2,OUTPUT);
  pinMode(DCM_EN1,OUTPUT);

  // init pin values
  digitalWrite(DCM_IN1,LOW);
  digitalWrite(DCM_IN2,LOW);
  digitalWrite(DCM_IN3,LOW);
  digitalWrite(DCM_IN4,LOW);

  // init pwm pins
  softPwmCreate (DCM_EN1, 0, 100);
  softPwmCreate (DCM_EN2, 0, 100);

}

// backward driving
void DriveMotor::backward() {

  softPwmWrite (DCM_EN1, this->dcm1val);
  softPwmWrite (DCM_EN2, this->dcm2val);

  digitalWrite(DCM_IN1,LOW);
  digitalWrite(DCM_IN2,HIGH);
  digitalWrite(DCM_IN3,LOW);
  digitalWrite(DCM_IN4,HIGH);

}


//
// forward driving
//
void DriveMotor::forward() {
  softPwmWrite (DCM_EN1, this->dcm1val);
  softPwmWrite (DCM_EN2, this->dcm2val);

  digitalWrite(DCM_IN1,HIGH);
  digitalWrite(DCM_IN2,LOW);
  digitalWrite(DCM_IN3,HIGH);
  digitalWrite(DCM_IN4,LOW);
}

// turn left
void DriveMotor::turn_left() {
  softPwmWrite (DCM_EN1, this->dcm1val);
  softPwmWrite (DCM_EN2, this->dcm2val);

  digitalWrite(DCM_IN1,LOW);
  digitalWrite(DCM_IN2,HIGH);
  digitalWrite(DCM_IN3,HIGH);
  digitalWrite(DCM_IN4,LOW);
}

// turn right
void DriveMotor::turn_right() {
  softPwmWrite (DCM_EN1, this->dcm1val);
  softPwmWrite (DCM_EN2, this->dcm2val);

  digitalWrite(DCM_IN1,HIGH);
  digitalWrite(DCM_IN2,LOW);
  digitalWrite(DCM_IN3,LOW);
  digitalWrite(DCM_IN4,HIGH);
}

//
// stop motor output
//
void DriveMotor::stop() {
  softPwmWrite (DCM_EN1, 0);
  softPwmWrite (DCM_EN2, 0);

  digitalWrite(DCM_IN1,LOW);
  digitalWrite(DCM_IN2,LOW);
  digitalWrite(DCM_IN3,LOW);
  digitalWrite(DCM_IN4,LOW);
}
