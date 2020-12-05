cmake_minimum_required(VERSION 3.10)

project(BeerBotClient VERSION 0.1
  DESCRIPTION "Simple Robot Learning Project.")

include_directories(/usr/include libraries/SFML-2.5.1/include/)

link_directories(libraries/SFML-2.5.1/lib/)
link_libraries(sfml-system sfml-window sfml-graphics)

file(GLOB SOURCES "src/*.cpp")

add_executable(BeerBotUI ${SOURCES})
