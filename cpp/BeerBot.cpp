#include <stdio.h>
#include <iostream>
#include <wiringPi.h>
#include "BeerBot.h"

// default constructor
BeerBot::BeerBot() {

}

// basic sensor and data initialization
void BeerBot::initialize() {
  std::cout << "initialize beerbot..." << std::endl;

  std::cout << "start eye calibration..." << std::endl;
  this->eye.calibration();
  std::cout << "eye calibration done." << std::endl;

  std::cout << "init done." << std::endl;
}

// simple attemp to drive without collision
void BeerBot::driveAround() {
  double mindist;
  long   waitval;

  mindist = this->eye.simple_scan(20);

  // drive 5 test lanes
  for (int i=0;i<5;i++) {

    // forward until blocked
    while (mindist > 24.0) {
      std::cout << mindist << std::endl;
      // step forward depends on mindist
      this->driver.forward();
      waitval = (long)(mindist * 90);
      if (waitval < 300) waitval = 300;
      delay(waitval);

      // stop motors
      this->driver.stop();
      delay(100);

      // check next distance value
      mindist = this->eye.simple_scan(20);
    } // forward until blocked

    std::cout << "turn around" << std::endl;
    // stop on block
    this->driver.backward();
    delay(300);
    this->driver.stop();

    // turn until enough space for next steps
    while (mindist < 60.0) {
      this->driver.turn_left();
      delay(350);
      this->driver.stop();
      mindist = eye.simple_scan(20);
    } // turn to space

  } // drive lanes

}
