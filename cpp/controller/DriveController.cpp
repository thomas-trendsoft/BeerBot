#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <wiringPi.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include "DriveController.h"

//
// default constructor
//
DriveController::DriveController() {
  this->dmpReady   = false;
  this->pos_x      = 0;
  this->pos_y      = 0;
  this->fstop       = 0;
}

//
// hardware init routine
//
void DriveController::initialize(EyeScanner* eye) {

  // save eye scanner ref
  this->eye = eye;

  this->pos_x = 0;
  this->pos_y = 0;

  // initialize device
  printf("Initializing MPU6050 devices...\n");
  mpu.initialize();

  // verify connection
  printf("Testing gyro device connections...\n");
  printf(mpu.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failed\n");

  // load and configure the DMP
  printf("Initializing DMP...\n");
  mpuStatus = mpu.dmpInitialize();

  // make sure it worked (returns 0 if so)
  if (mpuStatus == 0) {
      // turn on the DMP, now that it's ready
      printf("Enabling DMP...\n");
      mpu.setDMPEnabled(true);
      mpuIntStatus = mpu.getIntStatus();
      printf("DMP ready!\n");
      dmpReady = true;
      packetSize = mpu.dmpGetFIFOPacketSize();
      std::cout << "y acc offset: " << mpu.getYAccelOffset() << std::endl;

  } else {
      // ERROR!
      // 1 = initial memory load failed
      // 2 = DMP configuration updates failed
      // (if it's going to break, usually the code will be 1)
      printf("DMP Initialization failed (code %d)\n", mpuStatus);
  }

}

//
// show current position information
//
void DriveController::showStatus() {
  std::cout << "odo (l/r): " << odo_left.count() << "/" << odo_right.count() << std::endl;
}

void DriveController::updatePosition() {
  // orientation/motion vars
  Quaternion  q;           // [w, x, y, z]         quaternion container
  VectorFloat gravity;    // [x, y, z]            gravity vector
  float       ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

  int fifoCount = mpu.getFIFOCount();

  if (fifoCount == 1024) {
      // reset so we can continue cleanly
      mpu.resetFIFO();
      printf("FIFO overflow!\n");

  // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (fifoCount >= 42) {
      // work update position steps from fifo buffer
      while (fifoCount >= 42) {
        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);

        int pos = std::min(odo_left.count(),odo_right.count());

        // display Euler angles in degrees
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

        double c = cos(ypr[0]);
        double s = sin(ypr[0]);
        double delta = pos - this->last_odo_diff;
        this->last_odo_diff = pos;

        double dx = c * delta * ODO_TICK_LEN;
        double dy = s * delta * ODO_TICK_LEN;

        this->pos_x += dx;
        this->pos_y += dy;
        std::cout << "pos: " << this->pos_x << ":" << this->pos_y << ":" << (ypr[0]*180.0/M_PI) << "/" << delta << std::endl;

        fifoCount = mpu.getFIFOCount();
      }
  }
}

void DriveController::balance() {
  int lval = motors.getMotorLeftValue();
  int rval = motors.getMotorRightValue();
  int delta = odo_left.count() - odo_right.count();

  std::cout << "u:" << delta << "/" << this->last_odo_diff << " - " << lval << ":" << rval << std::endl;
  std::cout << "o l/r: " << odo_left.count() << "/" << odo_right.count() << std::endl;
  // test stay forward
  if (delta > 0) {
      if (lval > 35) {
        motors.setMotorLeftValue(lval-(2.0 * delta));
      } else if (rval <= 70) {
        motors.setMotorRightValue(rval+(2.0 * delta));
      }
  } else if (delta < 0) {
    if (rval > 35) {
      motors.setMotorRightValue(rval+(2.0 * delta));
    } else if (lval <= 70) {
      motors.setMotorLeftValue(lval-(2.0 * delta));
    }
  }
}

//
// drive forward for "dist" steps
//
void DriveController::forward(int dist) {
  fstop = 0;
  odo_left.reset();
  odo_right.reset();

  mpu.resetFIFO();
  this->last_odo_diff = 0;
  motors.forward();

  int    state = 4;
  double maxdist = this->eye->distance();
  std::cout << "try forward: " << dist << "/" << maxdist << std::endl;
  while (!fstop && maxdist > 12.0 && odo_left.count() < dist && odo_right.count() < dist) {
    delay(2);
    std::cout << ".";
    int lstate = odo_left.count() + odo_right.count();
    if (lstate > state) {
      //this->balance();
      this->updatePosition();
      maxdist = this->eye->distance();
      state = lstate + 2;
    }
  }

  std::cout << odo_left.count() << "/" << odo_right.count() << std::endl;

  motors.stop();
  std::cout << "end pos: " << this->pos_x << "/" << this->pos_y << std::endl;

}

//
// drive backward for "dist" steps
//
void DriveController::backward(int dist) {
  fstop = 0;
  odo_left.reset();
  odo_right.reset();

  motors.backward();
  while (!fstop && odo_left.count() < dist && odo_right.count() < dist) {
    delay(2);
  }
  motors.stop();
}

//
// turn left for given odo meter steps
//
void DriveController::turn_left(int steps) {
  fstop = 0;
  odo_left.reset();
  odo_right.reset();

  motors.turn_left();
  while (!fstop && odo_left.count() < steps && odo_right.count() < steps) {
    delay(2);
  }
  motors.stop();
}

//
// turn right for given odo meter steps
//
void DriveController::turn_right(int steps) {
  fstop = 0;
  odo_left.reset();
  odo_right.reset();

  motors.turn_right();
  while (!fstop && odo_left.count() < steps && odo_right.count() < steps) {
    delay(2);
  }
  motors.stop();
}

void DriveController::stopMove() {
  fstop = 1;
  motors.stop();
}
