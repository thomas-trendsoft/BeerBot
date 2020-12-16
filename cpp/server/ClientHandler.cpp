#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
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
  if (client != NULL) delete (int*)client;

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
  std::vector<char>          buf(5000);
  int                   rc;
  std::map<std::string,std::string> msg;

  // read first packet
  rc = recv(this->socket,buf.data(),buf.size(),0);
  if (rc < 6) {
    std::cout << "err read: " << rc << std::endl;
    this->errmsg = "no good handshake";
    this->error = -1;
    return msg;
  }

  std::cout << "read msg: " << buf.data() << std::endl;

  // tokenize
  char* token = strtok(buf.data(),";");
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
// basic message send to client for this handler
//
int ClientHandler::sendMessage(std::map<std::string,std::string> msg) {
  int clen = 0;

  std::map<std::string, std::string>::iterator it;

  std::stringstream ss;
  for (it = msg.begin(); it != msg.end(); it++)
  {
    clen += it->first.length();
    clen += it->second.length();
    ss << ";" << it->first << ";" << it->second;
  }

  // create output string
  std::stringstream data;

  data << "LEN;" << clen << ";" << ss.str() << std::endl;

  std::string dstr = data.str();

  // send msg
  return send(this->socket,dstr.data(),dstr.length(),0);

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
