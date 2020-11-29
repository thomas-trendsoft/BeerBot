#ifndef EYESCANNER_H_
#define EYESCANNER_H_

#include "Distance.h"
#include "StepMotor.h"

//
// eye scanner controller
//
class EyeScanner {

  // calvalue
  int calvalue = 405;

  // current step motor position
  int position;

  // distance sensor
  Distance dist;

  // step motor controller
  StepMotor stepper;

  public:
    // default constructor
    EyeScanner();

    // search 0 position
    bool calibration();

    // simple min dist scan for given steps
    double simple_scan(int steps);

};

#endif
