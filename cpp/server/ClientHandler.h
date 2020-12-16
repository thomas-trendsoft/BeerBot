#ifndef CLIENTHANDLER_H_
#define CLIENTHANDLER_H_

#include <map>
#include <vector>
#include <string>
#include <pthread.h>


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
  std::string errmsg;

public:
  // default constructor
  ClientHandler(int socket);

  // read basic msg
  std::map<std::string,std::string> readMessage();

  // send basic msg
  int sendMessage(std::map<std::string,std::string> msg);

  // welcome handshake for clients
  int handShake();

  // handle client commands
  void handle();

  // close connection and thread
  void shutdown();

};

#endif
