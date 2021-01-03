#ifndef CLIENTHANDLER_H_
#define CLIENTHANDLER_H_

#include <map>
#include <vector>
#include <string>
#include <thread>
#include "BeerBot.h"

#define NO_TASK    0
#define MAP_TASK   1
#define ORDER_TASK 2
#define STOP_TASKS -1

using namespace std;

// msg data entry
typedef struct msgdata_str {
  msgdata_str() : next(NULL) {}
  string key;
  string value;
  struct msgdata_str*   next;
} msgdata;

// client info
typedef struct {
  int      socket;
  BeerBot* bot;
} clientinfo;


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

  // long running task id
  int long_task_id;

public:
  // default constructor
  ClientHandler(int socket,BeerBot* bot);

  // read basic msg
  msgdata* readMessage();

  // send basic msg
  int sendMessage(msgdata* msg);

  // free memory msg
  void freeMsg(msgdata* msg);

  // welcome handshake for clients
  int handShake();

  // handle client commands
  void handle();

  // close connection and thread
  void shutdown();

  // move command execution
  void moveCommand(msgdata* msg);

  // active long running task
  int currentTask();

  // update running task
  void setTask(int id);

  BeerBot* getBot();

  void task_thread();

};

#endif
