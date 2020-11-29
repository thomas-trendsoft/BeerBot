#ifndef DRIVECONTROLLER_H_
#define DRIVECONTROLLER_H_

#define OUTPUT_READABLE_YAWPITCHROLL

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <chrono>
#include <wiringPi.h>
#include "MPU6050.h"
#include "DriveMotor.h"
#include "OdoMeter.h"

using namespace std::chrono;

//
// global drive controller
//
class DriveController {

  MPU6050 mpu;

  // odo meter right
  OdoMeter odo1 = OdoMeter(5);

  // odo meter left
  OdoMeter odo2 = OdoMeter(6);

  // motors interface
  DriveMotor motors;

  // dmp mpu flag
  bool dmpReady;

  // MPU device status
  uint8_t mpuStatus;

  // mpu interrupt status
  uint8_t mpuIntStatus;

  // mpu fifo packet size
  uint16_t packetSize;

  // FIFO storage buffer
  uint8_t fifoBuffer[64];

  // last position update
  system_clock::time_point pos_update;

  // estimated x position
  long pos_x;

  // estimated y position
  long pos_y;


public:
  // default constructor
  DriveController();

  // init driver controller
  void initialize();

  // try to be straight
  void balance();

  // util method to play with mpu data
  void updatePosition();

  // drive forward for n steps
  void forward(int dist);

  // drive backward for n steps
  void backward(int dist);

  void turn_left(int steps);

  void turn_right(int steps);

  void showStatus();
};

#endif
