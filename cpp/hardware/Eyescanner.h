#ifndef EYESCANNER_H_
#define EYESCANNER_H_

#define EYE_ECHO    17
#define EYE_TRIGGER 18

class EyeScanner {

  int calvalue;

  public:
    // default constructor
    EyeScanner();

    // measure simple distance
    double measure();

};

#endif
