#ifndef DRIVECONTROLLER_H_
#define DRIVECONTROLLER_H_

#define OUTPUT_READABLE_YAWPITCHROLL

#define WHEEL_LEN 23.0
#define ODO_TICKS 40.0
#define ODO_TICK_LEN (WHEEL_LEN / ODO_TICKS)

#define BAL_MAX_DIFF 14
#define BAL_REF_VAL  24

#define M_STOP     0
#define M_FORWARD  1
#define M_BACKWARD 2
#define M_LEFT     3
#define M_RIGHT    4


#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <chrono>
#include <mutex>
#include <wiringPi.h>
#include "helper_3dmath.h"
#include "MPU6050.h"
#include "Eyescanner.h"
#include "DriveMotor.h"
#include "OdoMeter.h"

using namespace std::chrono;

typedef struct {
	double x;
	double y;
	double theta;
} position;

//
// global drive controller
//
class DriveController {

  // gyro / accleration hardware
  MPU6050 mpu;

  // odo meter left
  OdoMeter odo_left = OdoMeter(5);

  // odo meter right
  OdoMeter odo_right = OdoMeter(6);

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

  // aktuelle position
  position pos;

  // last known odo positions
  int last_odo_diff;

  // eye scanner controller interface
  EyeScanner* eye;

  // stop the movement flag
  int fstop;

  // pos accessor mutex
  std::mutex posMutex;

  // position update thread
  pthread_t posThread;

	// last balance direction error
	double last_dir_err;

	// last direction balance update timestamp
	system_clock::time_point last_dir_update;

	// balance integral sum
	double balance_integral;

	// moving direction flag
	int mdir;

public:
  // default constructor
  DriveController();

  // init driver controller
  void initialize(EyeScanner* eye);

  // try to be straight
  void balance(double dir);

  // util method to play with mpu data
  void updatePosition();

  // drive forward for n steps
  void forward(int dist);

  // drive backward for n steps
  void backward(int dist);

  // turn in left direction
  void turn_left(int steps);

  // turn in right direction
  void turn_right(int steps);

  // disable dc motors
  void stopMove();

	// copy current position
	void getPosition(position* pos);

  // get current estimated position
  position getStatus();

	// reset 2D Position
	void resetPosition();

  // shutdown drive controller
  void shutdown();

};

#endif
