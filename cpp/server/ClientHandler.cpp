#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "ClientHandler.h"

//
// client thread method
//
void* client_handler_start(void* client) {
  int csocket = *((int*)client);

  ClientHandler chandler(csocket);

  // try handshake protocols
  if (chandler.handShake() < 0) {
    std::cout << "handshake with client failed." << std::endl;
    pthread_exit(NULL);
    return 0;
  }

  // handle polling commands
  chandler.handle();

  // close connection
  chandler.shutdown();

  // exit thread
  pthread_exit(NULL);

  return 0;
}

//
// default constructor
//
ClientHandler::ClientHandler(int socket) {
  this->socket = socket;
}

//
// read basic message from protocol
//
std::map<std::string,std::string> ClientHandler::readMessage() {
  char                  buffer[1024];
  int                   rc;
  std::map<std::string,std::string> msg;

  std::cout << "start read" << std::endl;
  // read first packet
  rc = recv(this->socket,buffer,1024,0);
  if (rc < 6) {
    this->errmsg = "no good handshake";
    this->error = -1;
    return msg;
  }

  std::cout << "read msg: " << buffer << std::endl;

  // tokenize
  char* token = strtok(buffer,";");
  if (token == NULL) {
    this->errmsg = "failed to parse message";
    this->error = -1;
    return msg;
  }

  // parse msg len and read further bytes if needed
  std::cout << "len msg: " << token << std::endl;
  int mlen = atoi(token);

  std::cout << "parsed: " << mlen << std::endl;

  return msg;
}

//
// protocol handshake
//
int ClientHandler::handShake() {
    std::cout << "open client connection..." << std::endl;
    std::map<std::string,std::string> msg = this->readMessage();

    return 0;
}

//
// client command handling
//
void ClientHandler::handle() {
  std::cout << "handle client..." << std::endl;
}

//
// close client connection
//
void ClientHandler::shutdown() {
  std::cout << "close client connection" << std::endl;
  close(this->socket);
}
