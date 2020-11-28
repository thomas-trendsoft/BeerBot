#ifndef ODOMETER_H_
#define ODOMETER_H_

class OdoMeter {
  int index;
public:
  // default constructor
  OdoMeter(int pin);

  // reset counter
  void reset();

  // read count
  long count();


};

#endif
