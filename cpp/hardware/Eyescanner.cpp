#include <chrono>
#include <wiringPi.h>
#include "Eyescanner.h"

//
// default constructor
//
EyeScanner::EyeScanner() {
}

// calibration
bool EyeScanner::calibration() {
  int i=0;
  double check = this->dist.measure();
  while (i < 200 && check > 8.0) {
    this->stepper.rotate(2,-1);
    check = this->dist.measure();
  }
  this->stepper.rotate(this->calvalue,1);
  position = 0;
  return i < 200;
}

// simple min dist scan for given steps
double EyeScanner::simple_scan(int steps) {
  double minval = 2000.0;
  double val;

  this->stepper.rotate(steps*10,1);
  for (int i=(0-steps);i<steps;i++) {
    val = this->dist.measure();
    if (val < minval) {
      minval = val;
    }
    this->stepper.rotate(10,-1);
  }
  this->stepper.rotate(steps*10,1);
  return minval;
}
