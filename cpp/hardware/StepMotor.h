#ifndef STEPMOTOR_H_
#define STEPMOTOR_H_

#define STEPPIN0 22
#define STEPPIN1 23
#define STEPPIN2 24
#define STEPPIN3 25

//
// StepMotor Controller
//
class StepMotor {

  int pins[4] = {STEPPIN0,STEPPIN1,STEPPIN2,STEPPIN3};

  // step motor output sequence
  int sequence[4][4] = {{1,1,0,0},{0,1,1,0},{0,0,1,1},{1,0,0,1}};

  // current sequence position
  int seqidx;

  public:
    // default constructor
    StepMotor();

    // rotate motor in given dir for steps
    void rotate(int steps,int dir);

    // stop voltage on step motor magnets
    void pause();

};

#endif
