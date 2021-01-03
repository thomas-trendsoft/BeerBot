#include "Logger.h"

std::mutex Logger::mtx;

void Logger::log(std::string msg) {
  mtx.lock();
  std::cout << msg << std::endl;
  mtx.unlock();
}
