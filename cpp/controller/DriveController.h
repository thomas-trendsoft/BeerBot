#ifndef DRIVECONTROLLER_H_
#define DRIVECONTROLLER_H_

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "MPU6050.h"
#include "OdoMeter.h"

class DriveController {

  MPU6050 mpu;

  OdoMeter odo1 = OdoMeter(5);

  OdoMeter odo2 = OdoMeter(6);

public:
  DriveController();

  void showStatus();
};

#endif
