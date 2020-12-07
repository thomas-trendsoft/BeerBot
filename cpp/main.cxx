#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "Eyescanner.h"
#include "BeerBot.h"
#include "BeerBotServer.h"
#include <wiringPi.h>

int main() {

  std::cout << "Start Base Setup..." << std::endl;

  wiringPiSetupGpio();

  //BeerBot bbot = BeerBot();
  //bbot.initialize();
  //bbot.driveAround();

  std::cout << "Starte BeerBot Server..." << std::endl;
  BeerBotServer server = BeerBotServer();
  server.start();

  // DriveController driver = DriveController();
  // EyeScanner      eye    = EyeScanner();
  //
  // driver.initialize(&eye);
  //
  // std::cout << "start demo.." << std::endl;
  //
  // double* data = eye.scan(60);
  //
  // for (int i=0;i<120;i++) {
  //   std::cout << data[i] << std::endl;
  // }
  //
  // delete[] data;

  delay(20000);

  server.stop();

  delay(5000);

  return 0 ;

}
