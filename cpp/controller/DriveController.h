#ifndef DRIVECONTROLLER_H_
#define DRIVECONTROLLER_H_

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "MPU6050.h"
#include "DriveMotor.h"
#include "OdoMeter.h"

//
// global drive controller
//
class DriveController {

  MPU6050 mpu;

  // odo meter
  OdoMeter odo1 = OdoMeter(5);

  // odo meter interface
  OdoMeter odo2 = OdoMeter(6);

  // motors interface
  DriveMotor motors;

public:
  // default constructor
  DriveController();

  // drive forward for n steps
  void forward(int dist);

  // drive backward for n steps
  void backward(int dist);

  void turn_left(int steps);

  void turn_right(int steps);

  void showStatus();
};

#endif
