#ifndef DRIVEMOTOR_H_
#define DRIVEMOTOR_H_

#define DCM_IN1 1
#define DCM_IN2 7
#define DCM_IN3 11
#define DCM_IN4 10
#define DCM_EN2 9
#define DCM_EN1 0

//
// driving motor Controller
//
class DriveMotor {

  int dcm1val = 40; // left motor

  int dcm2val = 55; // right motor

  public:
    // default constructor
    DriveMotor();

    // stop driving
    void stop();

    // forward driving
    void forward();

    // backward driving
    void backward();

    // turn left
    void turn_left();

    // turn right
    void turn_right();


};

#endif
