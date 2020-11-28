cmake_minimum_required(VERSION 3.10)

project(BeerBot VERSION 0.1
  DESCRIPTION "Simple Robot Learning Project.")

include_directories(mpu6050 hardware controller /usr/include)
link_libraries(wiringPi)
link_libraries(pthread)

add_executable(BeerBot main.cxx controller/DriveController.cpp hardware/OdoMeter.cpp mpu6050/I2Cdev.cpp mpu6050/MPU6050.cpp hardware/Eyescanner.cpp hardware/DriveMotor.cpp hardware/StepMotor.cpp hardware/Distance.cpp BeerBot.cpp)
