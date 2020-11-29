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
  this->pos_update = system_clock::now();
}

//
// hardware init routine
//
void DriveController::initialize() {
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
  std::cout << "odo1: " << odo1.count() << std::endl;
  std::cout << "odo2: " << odo2.count() << std::endl;
}

void DriveController::updatePosition() {
  // orientation/motion vars
  Quaternion q;           // [w, x, y, z]         quaternion container
  VectorInt16 aa;         // [x, y, z]            accel sensor measurements
  VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
  VectorFloat gravity;    // [x, y, z]            gravity vector
  float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

  system_clock::time_point start = system_clock::now();

  int fifoCount = mpu.getFIFOCount();

  if (fifoCount == 1024) {
      // reset so we can continue cleanly
      mpu.resetFIFO();
      printf("FIFO overflow!\n");

  // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (fifoCount >= 42) {
      while (fifoCount >= 42) {
        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);

        // display Euler angles in degrees
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetAccel(&aa, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
        //std::cout << pos_update << std::endl;
        printf("ypr  %7.2f %7.2f %7.2f    \n", ypr[0] * 180/M_PI, ypr[1] * 180/M_PI, ypr[2] * 180/M_PI);
        printf("areal %6d %6d %6d    \n", aaReal.x, aaReal.y, aaReal.z);
        printf("%.2f dirvec",sqrt(pow(aaReal.x,2.0)+pow(aaReal.y,2.0)));
        printf("\n");
        pos_update = start;
        fifoCount = mpu.getFIFOCount();
      }
  }
}

void DriveController::balance() {
  int lval = motors.getMotorLeftValue();
  int rval = motors.getMotorRightValue();
  int delta = odo1.count() - odo2.count();

  std::cout << "u:" << delta << "/" << lval << ":" << rval << std::endl;

  // test stay forward
  if (delta > 0) {
      if (lval < 50) {
        motors.setMotorLeftValue(lval+1);
      } else {
        motors.setMotorRightValue(rval-1);
      }
  } else if (delta < 0) {
    if (rval < 50) {
      motors.setMotorRightValue(rval+1);
    } else {
      motors.setMotorLeftValue(lval-1);
    }
  }
}

//
// drive forward for "dist" steps
//
void DriveController::forward(int dist) {
  odo1.reset();
  odo2.reset();
  int delta;

  std::cout << "try forward: " << dist << std::endl;

  motors.forward();
  int state = 4;
  while (odo1.count() < dist && odo2.count() < dist) {
    delay(2);
    int lstate = odo1.count() + odo2.count();
    if (lstate > state) {
      //this->balance();
      this->updatePosition();
      state = lstate + 5;
    }
  }

  motors.stop();

}

//
// drive backward for "dist" steps
//
void DriveController::backward(int dist) {
  odo1.reset();
  odo2.reset();

  motors.backward();
  while (odo1.count() < dist && odo2.count() < dist) {
    delay(2);
  }
  motors.stop();
}

//
// turn left for given odo meter steps
//
void DriveController::turn_left(int steps) {
  odo1.reset();
  odo2.reset();

  motors.turn_left();
  while (odo1.count() < steps && odo2.count() < steps) {
    delay(2);
  }
  motors.stop();
}

//
// turn right for given odo meter steps
//
void DriveController::turn_right(int steps) {
  odo1.reset();
  odo2.reset();

  motors.turn_right();
  while (odo1.count() < steps && odo2.count() < steps) {
    delay(2);
  }
  motors.stop();
}
