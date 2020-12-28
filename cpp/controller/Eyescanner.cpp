#include <iostream>
#include <chrono>
#include <math.h>
#include <wiringPi.h>
#include "Eyescanner.h"

//
// default constructor
//
EyeScanner::EyeScanner() {
	position = 0;
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
  this->stepper.pause();
  position = 0;
  return i < 200;
}

// simple distance measurement
double EyeScanner::distance() {
  return this->dist.measure();
}


//
// make a full scan with all values return
//
ts_scan_t* EyeScanner::scan(double x,double y,double theta) {
	double     sval;
	ts_scan_t* scan = new ts_scan_t;

	int steps = 70;

	double ldir = theta - (steps * DELTA_ANGEL * 5);
	if (ldir < -M_PI) {
		ldir += (2*M_PI);
	}

  this->stepper.rotate(steps*5,1);

	int scanidx = 0;
  for (int i=(0-steps);i<steps;i++) {
    sval = this->dist.measure();

		double c = cos(ldir);
		double s = sin(ldir);

		double dx = c * sval + x;
		double dy = s * sval + y;

		scan->x[scanidx] = dx;
		scan->y[scanidx] = dy;
		scan->value[scanidx] = TS_OBSTACLE;

		scanidx++;

//		std::cout << "s" << sval << ": " << i << ": " << scanidx <<  "  -  " << (ldir * 180.0 / M_PI)  << std::endl;

		ldir += DELTA_ANGEL*5;
		if (ldir > M_PI) {
			ldir -= (2*M_PI);
		}

    this->stepper.rotate(5,-1);
  }

	// turn back to center
  this->stepper.rotate(steps*5,1);

	// turn off stepper motor
  this->stepper.pause();

  return scan;
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
  this->stepper.pause();
  return minval;
}
