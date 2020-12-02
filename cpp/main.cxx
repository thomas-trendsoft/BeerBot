#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "Eyescanner.h"
#include "BeerBot.h"
#include <wiringPi.h>

int main() {

  std::cout << "Start Base Setup..." << std::endl;

  wiringPiSetupGpio();

  //BeerBot bbot = BeerBot();
  //bbot.initialize();
  //bbot.driveAround();

  DriveController driver = DriveController();
  EyeScanner      eye    = EyeScanner();

  driver.initialize(&eye);

  std::cout << "start demo.." << std::endl;

  double* data = eye.scan(25);

  for (int i=0;i<50;i++) {
    std::cout << data[i] << std::endl;
  }

  delete data;

  return 0 ;

}
