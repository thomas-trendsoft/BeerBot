#ifndef DISTANCE_H_
#define DISTANCE_H_

#define DIST_ECHO    17
#define DIST_TRIGGER 18


//
// HC-SR04 Distance measurement
//
class Distance {
  public:
    // Default constructor
    Distance();

    // measure current distance
    double measure();
};

#endif
