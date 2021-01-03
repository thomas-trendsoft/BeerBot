#ifndef BEERBOT_H_
#define BEERBOT_H_

#include "TinySLAM.h"
#include "Eyescanner.h"
#include "DriveMotor.h"
#include "DriveController.h"

//
// main controller for the beerbot
//
class BeerBot {

  // SLAM implementation for mapping
  TinySLAM mapping;

  // distance sensor scanner
  EyeScanner eye;

  // motor drive controller
  DriveController driver;

  public:
    // default constructor
    BeerBot();

    // prepare bot running
    void initialize();

    // first simple drive attemp method
    void driveAround();

    // explore environment and create map
    void exploreMap();

    // drive forward
    void forward();

    // drive backward
    void backward();

    // turn left around
    void turnLeft();

    // turn left around
    void turnRight();

    // current distance measurement
    double checkDistance();

    // execute eye calibration
    void eyeCalibration();

    // aktuelle Position abfragen
    void currentPos(bb_position* pos);

    // stop movement
    void stop();

    // stop beer bot program
    void shutdown();

    // map accessor
    TinySLAM getMap();

};

#endif
