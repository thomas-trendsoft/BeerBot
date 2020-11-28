#include <iostream>
#include <wiringPi.h>
#include "DriveController.h"

//
// default constructor
//
DriveController::DriveController() {

}

//
// show current position information
//
void DriveController::showStatus() {
  std::cout << "odo1: " << odo1.count() << std::endl;
  std::cout << "odo2: " << odo2.count() << std::endl;
}

//
// drive forward for "dist" steps
//
void DriveController::forward(int dist) {
  odo1.reset();
  odo2.reset();

  motors.forward();
  while (odo1.count() < dist && odo2.count() < dist) {
    delay(2);
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
