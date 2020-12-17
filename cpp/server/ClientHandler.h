#ifndef CLIENTHANDLER_H_
#define CLIENTHANDLER_H_

#include <map>
#include <vector>
#include <string>
#include <pthread.h>

using namespace std;

typedef struct {
  string* key;
  string* value;
} msgdata;

void* client_handler_start(void* client);

//
// client handle routines
//
class ClientHandler {

  // tcp socket
  int socket;

  // error status
  int error;

  // error msg
  string errmsg;

public:
  // default constructor
  ClientHandler(int socket);

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

};

#endif
