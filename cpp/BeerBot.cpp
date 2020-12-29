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

}

// map accessor
TinySLAM BeerBot::getMap() {
	return this->mapping;
}

void BeerBot::eyeCalibration() {
  std::cout << std::endl << "start eye calibration..." << std::endl;
  this->eye.calibration();
  std::cout << "eye calibration done." << std::endl;
}

// drive forward
void BeerBot::forward() {
  this->driver.forward(200);
}

// drive backward
void BeerBot::backward() {
  this->driver.backward(30);
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

// stop beer bot program
void BeerBot::shutdown() {
	this->driver.stopMove();
	this->driver.shutdown();
}

// current distance measurement
double BeerBot::checkDistance() {
  return this->eye.simple_scan(20);
}

// aktuelle Position abfragen
position BeerBot::currentPos() {
	return this->driver.getStatus();
}

// create a map of the current environment
void BeerBot::exploreMap() {
  ts_position_t map_pos;

  std::cout << "start explore map" << std::endl;
  driver.resetPosition();

  mapping.init_map();
  std::cout << "init empty map..." << std::endl;

  // first scan
  position   pos;
  driver.getPosition(&pos);

  ts_scan_t* scan = eye.scan(pos.x,pos.y,pos.theta);

  // init map values with first scan
  // TODO check one position struct only and
  map_pos.x     = pos.x;
  map_pos.y     = pos.y;
  map_pos.theta = pos.theta;

  mapping.map_update(scan,&map_pos,124);

  delete scan;

  std::cout << "mapping done." << std::endl;
}


// simple attemp to drive without collision
void BeerBot::driveAround() {
  double mindist;
  long   waitval;

  mindist = this->eye.simple_scan(20);

  // drive 5 test lanes
  for (int i=0;i<3;i++) {

    // forward until blocked
    while (mindist > 28.0) {
      std::cout << mindist << std::endl;

      // step forward depends on mindist
      this->driver.forward((int)(mindist * 2));
      driver.getStatus();

      // check next distance value
      mindist = this->eye.simple_scan(20);
    } // forward until blocked

    std::cout << "turn around" << std::endl;
    // stop on block
    this->driver.backward(10);

    // turn until enough space for next steps
    while (mindist < 50.0) {
      this->driver.turn_left(15);
      mindist = eye.simple_scan(20);
    } // turn to space

  } // drive lanes

}
