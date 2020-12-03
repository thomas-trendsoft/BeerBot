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

  TinySLAM mapping;

  EyeScanner eye;

  DriveController driver;

  public:
    // default constructor
    BeerBot();

    // prepare bot running
    void initialize();

    // first simple drive attemp method
    void driveAround();

};

#endif
