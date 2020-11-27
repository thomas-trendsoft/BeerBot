#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Eyescanner.h"
#include "BeerBot.h"
#include <wiringPi.h>

int main() {

  std::cout << "Start Base Setup..." << std::endl;

  wiringPiSetupGpio();

  BeerBot bb = BeerBot();

  bb.initialize();
  bb.driveAround();

  return 0 ;

}
