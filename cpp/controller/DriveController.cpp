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

// TODO check interrupt controlled
void* update_position_thread(void* ctrlptr) {
	DriveController* ctrl = (DriveController*)ctrlptr;

	while (!stop_pos_thread) {
		ctrl->updatePosition();
		delay(10);
	}

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
	this->mdir          = -1;
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

// set current position to 0,0
void DriveController::resetPosition() {
	pos.x = 0.0;
	pos.y = 0.0;
}

//
// show current position information
//
bb_position DriveController::getStatus() {
  return pos;
}

void DriveController::getPosition(bb_position* p) {
	p->x     = pos.x;
	p->y     = pos.y;
	p->theta = pos.theta;
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

        // display Euler angles in degrees
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

				// update looking direction
				this->pos.theta = ypr[0];

				// get odo position
				int pos   = std::min(odo_left.count(),odo_right.count());
				int delta = pos - this->last_odo_diff;

				this->last_odo_diff = pos;

				if (delta > 0) {
					if (this->mdir == M_FORWARD) {
						// calculate direction factors
		        double c = cos(ypr[0]);
		        double s = sin(ypr[0]);

						double dx = c * delta * ODO_TICK_LEN;
		        double dy = s * delta * ODO_TICK_LEN;

						this->pos.x += dx;
						this->pos.y += dy;
					}
				}

        fifoCount = mpu.getFIFOCount();
      }
  }
}

void DriveController::balance(double dir) {
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
	this->balance_integral = this->balance_integral + (err * dt);

	double kpu = Kp * err;
	double kiu = Ki * this->balance_integral;
	double kdu = Kd * derr;

	int update = std::abs((int)(kpu + kiu + kdu));

	// not maximize diff to avoid drifting
	if (update > BAL_MAX_DIFF) update = BAL_MAX_DIFF;

  if (update != 0) {
		if (err < 0) {
	  	motors.setMotorLeftValue(BAL_REF_VAL - update);
	  	motors.setMotorRightValue(BAL_REF_VAL + update);
  	} else if (err > 0) {
	  	motors.setMotorLeftValue(BAL_REF_VAL + update);
	  	motors.setMotorRightValue(BAL_REF_VAL - update);
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

	// save current direction
  double direction = this->pos.theta;

	// init PID controller
	this->last_dir_err     = 0.0;
	this->last_dir_update  = system_clock::now();
	this->balance_integral = 0.0;

	// activate position updates
	this->last_odo_diff = 0;
	this->mdir = M_FORWARD;

  double maxdist = this->eye->distance();

	// start drive
	motors.forward();

	// drive until stop signal / distance reached / or occupied
  while (!fstop && maxdist > 24.0 && odo_left.count() < dist && odo_right.count() < dist) {
    delay(40);
    this->balance(direction);
    maxdist = this->eye->distance();
  }

	motors.stop();
	delay(200);


}

//
// drive backward for "dist" steps
//
void DriveController::backward(int dist) {
  fstop = 0;
  odo_left.reset();
  odo_right.reset();

	this->mdir = M_BACKWARD;

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

	this->mdir = M_LEFT;

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

	this->mdir = M_RIGHT;

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
