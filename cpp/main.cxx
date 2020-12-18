#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "Eyescanner.h"
#include "BeerBot.h"
#include "BeerBotServer.h"
#include <wiringPi.h>

int runApp = 1;

void keyboard_int_handler(int s){
  printf("Caught signal %d\n",s);
  runApp = 0;
}

int main() {

  std::cout << "Start Base Setup..." << std::endl;

  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = keyboard_int_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);

  std::cout << "Keyboard Interrupt handler registered." << std::endl;

  wiringPiSetupGpio();

  std::cout << "Raspberry Pi GPIO Setup done." << std::endl;

  //BeerBot bbot = BeerBot();
  //bbot.initialize();
  //bbot.driveAround();

  std::cout << "Starte BeerBot Server..." << std::endl;
  BeerBotServer server = BeerBotServer();
  server.start();

  while (runApp == 1) {
    delay(1000);
    std::cout << ".";
  }

  server.stop();

  delay(200);

  return 0 ;

}
