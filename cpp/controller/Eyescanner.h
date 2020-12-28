#ifndef EYESCANNER_H_
#define EYESCANNER_H_

#include "TinySLAM.h"
#include "Distance.h"
#include "StepMotor.h"

#define DELTA_ANGEL (M_PI / 1024.0)

//
// eye scanner controller
//
class EyeScanner {

  // calvalue
  int calvalue = 390;

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

    // simple distance measurement
    double distance();

    // simple min dist scan for given steps
    double simple_scan(int steps);

    // create a scan array for current position
    ts_scan_t* scan(double x,double y,double theta);

};

#endif
