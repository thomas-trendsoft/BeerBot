#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <wiringPi.h>
#include "MPU6050_6Axis_MotionApps20.h"
#include "DriveController.h"

int stop_pos_thread = 0;

void* update_position_thread(void* ctrlptr) {
	DriveController* ctrl = (DriveController*)ctrlptr;

	std::cout << "start position thread" << std::endl;

	while (!stop_pos_thread) {
		ctrl->updatePosition();
		delay(10);
	}

	std::cout << "stop position thread" << std::endl;

	pthread_exit(NULL);
}

//
// default constructor
//
DriveController::DriveController() {
  this->dmpReady      = false;
  this->pos.x         = 0.0;
  this->pos.y         = 0.0;
  this->pos.theta     = 0.0;
  this->fstop         = 0;
  this->eye           = NULL;
  this->last_odo_diff = 0;
}

//
// hardware init routine
//
void DriveController::initialize(EyeScanner* eye) {

  // save eye scanner ref
  this->eye = eye;

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

      delay(10);
      this->updatePosition();
  } else {
      // ERROR!
      // 1 = initial memory load failed
      // 2 = DMP configuration updates failed
      // (if it's going to break, usually the code will be 1)
      printf("DMP Initialization failed (code %d)\n", mpuStatus);
  }

  // start position thread
  pthread_create(&this->posThread,NULL,&update_position_thread,(void*)this);

}

//
// show current position information
//
position DriveController::getStatus() {
  std::cout << "odo (l/r): " << odo_left.count() << "/" << odo_right.count() << std::endl;
  return pos;
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

        this->pos.x += dx;
        this->pos.y += dy;
        this->pos.theta = ypr[0];

        fifoCount = mpu.getFIFOCount();
      }
  }
}

void DriveController::balance(double dir) {
  int lval = motors.getMotorLeftValue();
  int rval = motors.getMotorRightValue();

	system_clock::time_point tm = system_clock::now();

  double Kp = 1.4;
	double Ki = 0.0;
	double Kd = 2.8;

  double mp = this->pos.theta * 180.0 / M_PI;
  double dv = dir * 180.0 / M_PI;

  double err  = dv - mp;

	if (std::abs(err) > 180.0) {
	  if (err > 0) {
		  err = err - 360.0;
	  } else {
		  err = 360.0 + err;
	  }
  }

	double derr = err - last_dir_err;
	last_dir_err = err;
	double dt = (tm - this->last_dir_update).count();
	this->balance_integral = this->balance_integral + err * dt;

	double kpu = Kp * err;
	double kiu = Ki * this->balance_integral;
	double kdu = Kd * derr;
  std::cout << "ERROR: " << err << "(" << mp << "/" << dv << "/" << last_dir_err << ")" <<std::endl;

	int update = std::abs((int)(kpu + kiu + kdu));

	std::cout << "Kp = " << kpu << "/ Ki = " << kiu << "/ Kd = " << kdu << " --> " << update << std::endl;

	// not maximize diff to avoid drifting
	if (update > BAL_MAX_DIFF) update = BAL_MAX_DIFF;

  if (std::abs(err) < 2) {
	  motors.setMotorLeftValue(BAL_REF_VAL);
	  motors.setMotorRightValue(BAL_REF_VAL);
  } else if (err < 0) {
		std::cout << "L: " << update << std::endl;
	  motors.setMotorLeftValue(BAL_REF_VAL - update);
	  motors.setMotorRightValue(BAL_REF_VAL + update);
  } else if (err > 0) {
		std::cout << "R: " << update << std::endl;
	  motors.setMotorLeftValue(BAL_REF_VAL + update);
	  motors.setMotorRightValue(BAL_REF_VAL - update);
  }

  // next step pid
  //int u = (int)(Kp * err);


}

//
// drive forward for "dist" steps
//
void DriveController::forward(int dist) {
  fstop = 0;
  odo_left.reset();
  odo_right.reset();

  this->last_odo_diff = 0;
  motors.forward();

  double direction = this->pos.theta;

	// init PID controller
	this->last_dir_err     = 0.0;
	this->last_dir_update  = system_clock::now();
	this->balance_integral = 0.0;

  double maxdist = this->eye->distance();
  std::cout << "try forward: " << dist << "/" << maxdist << "/" << direction << std::endl;
  while (!fstop && maxdist > 24.0 && odo_left.count() < dist && odo_right.count() < dist) {
    delay(40);
    this->balance(direction);
    maxdist = this->eye->distance();
    std::cout << "mindist: " << maxdist << std::endl;
  }

  std::cout << odo_left.count() << "/" << odo_right.count() << std::endl;

  motors.stop();
  std::cout << "end pos: " << this->pos.x << "/" << this->pos.y << std::endl;

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

void DriveController::shutdown() {
	stop_pos_thread = 1;
	delay(20);
	mpu.reset();
}
