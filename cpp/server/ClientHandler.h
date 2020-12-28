#ifndef CLIENTHANDLER_H_
#define CLIENTHANDLER_H_

#include <map>
#include <vector>
#include <string>
#include <pthread.h>
#include "BeerBot.h"

#define NO_TASK    0
#define MAP_TASK   1
#define ORDER_TASK 2
#define STOP_TASKS -1

using namespace std;

// msg data entry
typedef struct {
  string* key;
  string* value;
} msgdata;

// client info
typedef struct {
  int      socket;
  BeerBot* bot;
} clientinfo;

void* client_handler_start(void* client);

//
// client handle routines
//
class ClientHandler {

  // tcp socket
  int socket;

  // bot instance
  BeerBot* bot;

  // error status
  int error;

  // error msg
  string errmsg;

  // task thread
  pthread_t taskthread;

public:
  // default constructor
  ClientHandler(int socket,BeerBot* bot);

  // read basic msg
  map<string,msgdata*>* readMessage();

  // send basic msg
  int sendMessage(map<string,string> msg);

  // free memory msg
  void freeMsg(map<string,msgdata*>* msg);

  // welcome handshake for clients
  int handShake();

  // handle client commands
  void handle();

  // close connection and thread
  void shutdown();

  // check if task thread is running
  void checkTaskThread();

  // move command execution
  void moveCommand(map<string,msgdata*> msg);

};

#endif
