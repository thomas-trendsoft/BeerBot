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

  driver.initialize(&this->eye);

  std::cout << std::endl << "start eye calibration..." << std::endl;
  this->eye.calibration();
  std::cout << "eye calibration done." << std::endl;

  std::cout << "init empty map..." << std::endl;

  mapping.init_map();

  std::cout << "init done." << std::endl;
}

// drive forward
void BeerBot::forward() {
  this->driver.forward(30);
}

// drive backward
void BeerBot::backward() {
  this->driver.forward(30);
}

// turn left around
void BeerBot::turnLeft() {
  this->driver.turn_left(10);
}

// turn left around
void BeerBot::turnRight() {
  this->driver.turn_right(10);
}

// stop movement
void BeerBot::stop() {
  this->driver.stopMove();
}

// current distance measurement
double BeerBot::checkDistance() {

}


// simple attemp to drive without collision
void BeerBot::driveAround() {
  double mindist;
  long   waitval;

  mindist = this->eye.simple_scan(20);

  // drive 5 test lanes
  for (int i=0;i<5;i++) {

    // forward until blocked
    while (mindist > 28.0) {
      std::cout << mindist << std::endl;

      // step forward depends on mindist
      this->driver.forward((int)(mindist / 2.3));
      driver.showStatus();

      // check next distance value
      mindist = this->eye.simple_scan(20);
    } // forward until blocked

    std::cout << "turn around" << std::endl;
    // stop on block
    this->driver.backward(10);

    // turn until enough space for next steps
    while (mindist < 50.0) {
      this->driver.turn_left(10);
      mindist = eye.simple_scan(20);
    } // turn to space

  } // drive lanes

}
