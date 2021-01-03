#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <mutex>
#include <iostream>

class Logger {


public:
  static std::mutex mtx;

  static void log(std::string msg);

};

#endif
