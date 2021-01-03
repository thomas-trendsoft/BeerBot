#include <stdio.h>
#include <iostream>
#include <wiringPi.h>
#include "BeerBot.h"
#include "Logger.h"


// default constructor
BeerBot::BeerBot() {

}

// basic sensor and data initialization
void BeerBot::initialize() {
  Logger::log("initialize beerbot...");

  driver.initialize(&this->eye);

}

// map accessor
TinySLAM BeerBot::getMap() {
	return this->mapping;
}

void BeerBot::eyeCalibration() {
  Logger::log("start eye calibration...");
  this->eye.calibration();
  Logger::log("eye calibration done.");
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
  this->driver.turn_left(15);
}

// turn left around
void BeerBot::turnRight() {
  this->driver.turn_right(15);
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
void BeerBot::currentPos(bb_position* pos) {
	this->driver.getPosition(pos);
}

// create a map of the current environment
void BeerBot::exploreMap() {
  ts_position_t map_pos;

  Logger::log("start explore map");
  driver.resetPosition();

  mapping.init_map();
  Logger::log("init empty map...");

  // first scan
  bb_position   pos;
  driver.getPosition(&pos);

  ts_scan_t* scan = eye.scan(pos.x,pos.y,pos.theta);

  // init map values with first scan
  // TODO check one position struct only and
  map_pos.x     = pos.x;
  map_pos.y     = pos.y;
  map_pos.theta = pos.theta;

  mapping.map_update(scan,&map_pos,124);

  delete scan;

  Logger::log("mapping done.");
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

      // step forward depends on mindist
      this->driver.forward((int)(mindist * 2));
      driver.getStatus();

      // check next distance value
      mindist = this->eye.simple_scan(20);
    } // forward until blocked

    Logger::log("turn around");
    // stop on block
    this->driver.backward(10);

    // turn until enough space for next steps
    while (mindist < 50.0) {
      this->driver.turn_left(15);
      mindist = eye.simple_scan(20);
    } // turn to space

  } // drive lanes

}
