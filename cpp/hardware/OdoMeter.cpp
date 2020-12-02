#include "OdoMeter.h"
#include <wiringPi.h>

long globalCounter[2];

void odometer1ISR(void) {
  ++globalCounter[0];
}

void odometer2ISR(void) {
  ++globalCounter[1];
}

OdoMeter::OdoMeter(int pin) {
  if (pin == 5) {
    wiringPiISR(pin,INT_EDGE_BOTH,&odometer1ISR);
    this->index = 0;
  } else if (pin == 6) {
    wiringPiISR(pin,INT_EDGE_BOTH,&odometer2ISR);
    this->index = 1;
  } else {
    this->index = -1;
  }
}

void OdoMeter::reset() {
  if (this->index >= 0)
    globalCounter[this->index] = 0L;
}

long OdoMeter::count() {
  if (this->index >= 0) {
    return globalCounter[this->index];
  } else {
    return -1;
  }
}
