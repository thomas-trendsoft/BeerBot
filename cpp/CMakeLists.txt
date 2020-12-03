cmake_minimum_required(VERSION 3.10)

project(BeerBot VERSION 0.1
  DESCRIPTION "Simple Robot Learning Project.")

include_directories(mpu6050 hardware controller mapping /usr/include)
link_libraries(wiringPi)
link_libraries(pthread)

file(GLOB SOURCES "hardware/*.cpp" "controller/*.cpp" "mpu6050/*.cpp" "mapping/*.cpp")

add_executable(BeerBot main.cxx BeerBot.cpp ${SOURCES})
