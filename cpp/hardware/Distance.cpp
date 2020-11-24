#include <chrono>
#include <wiringPi.h>
#include "Distance.h"

using namespace std::chrono;

// default constructor
Distance::Distance() {
  // setup gpio pins
  pinMode(DIST_TRIGGER,OUTPUT);
  pinMode(DIST_ECHO,INPUT);
}

//
// measure
// ------------------------
//
// simple one measurement of the current distance
//
double Distance::measure() {

  // start measure signal
  digitalWrite (DIST_TRIGGER,1);
  delay(100);
  digitalWrite (DIST_TRIGGER,0);

  // save time stamp
  system_clock::time_point start = system_clock::now();
  system_clock::time_point end   = system_clock::now();

  // wait for response to start
  int ev;
  do {
    start = system_clock::now();
    ev = digitalRead(DIST_ECHO);
  } while (ev == 0);

  do {
    end = system_clock::now();
    ev  = digitalRead(DIST_ECHO);
  } while (ev == 1);

  long elapsed = duration_cast<std::chrono::milliseconds>(end - start).count();

  // mit der Schallgeschwindigkeit (34300 cm/s) multiplizieren
  // und durch 2 teilen, da hin und zurueck
  double distance = (elapsed * 34300) / 2.0;

  return distance;
}
