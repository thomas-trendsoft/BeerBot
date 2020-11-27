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

  //MPU6050 mpu = MPU6050();
  //mpu.dmpInitialize();

  //std::cout << "measurement: " << mpu.testConnection() << std::endl;

  BeerBot bbot = BeerBot();
  bbot.initialize();
  bbot.driveAround();

  return 0 ;

}
