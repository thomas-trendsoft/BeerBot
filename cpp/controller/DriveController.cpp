#include <iostream>
#include "DriveController.h"

DriveController::DriveController() {

}

void DriveController::showStatus() {
  std::cout << "odo1: " << odo1.count() << std::endl;
  std::cout << "odo2: " << odo2.count() << std::endl;
}
