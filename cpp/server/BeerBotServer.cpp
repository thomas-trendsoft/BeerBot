#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include "BeerBotServer.h"
#include "ClientHandler.h"
#include "Logger.h"

void BeerBotServer::main_thread() {

  Logger::log("start wait for client...");
  while (!isStopped()) {
    int cc = acceptClient();
    Logger::log(string("accepted client...") + std::to_string(cc) );
  }

}

//
// client thread method
//
void BeerBotServer::handle_client_thread(int cs) {
  ClientHandler chandler(cs,bot);

  // try handshake protocols
  if (chandler.handShake() < 0) {
    Logger::log("handshake with client failed.");
    return;
  }

  // handle polling commands
  chandler.handle();

  // close connection
  chandler.shutdown();

  // free client info memory
  delay(3000);

  Logger::log("thread ist out");

}

//
// open a new client connection
//
int BeerBotServer::acceptClient() {
  int clientSocket;
  struct timeval tv;

  struct sockaddr  client_addr;
  socklen_t        client_addr_size = sizeof(client_addr);

  tv.tv_sec = (long)5;
  tv.tv_usec = 0;

  if ((clientSocket = accept(this->serverSocket,(struct sockaddr *) &client_addr,&client_addr_size))<0)
  {
    perror("accept failed for client connection");
    return -1;
  }

  // check client socket state
  if (clientSocket > 0) {
    std::thread( [this,clientSocket] { this->handle_client_thread(clientSocket); }).detach();

    return 1;
  } else if (clientSocket < 0) {
    Logger::log("failed to select client socket");
    return -1;
  }

  // timeout is ok if no client tried to connect
  return 0;
}

//
// start server listening and client handling
//
int BeerBotServer::start() {

  // set client thread param index
  this->clientIdx = 0;

  // check socket init
  if (this->serverSocket < 0) {
    return -1;
  }

  // listen for connections
  if (listen(this->serverSocket,3) < 0) {
    Logger::log("ERROR on listen to server port");
    return -1;
  }

  // server thread start
  std::thread( [this] { this->main_thread(); }).detach();

  this->stopServer = false;

  return 0;
}

//
// check server running status flag
//
int BeerBotServer::isStopped() {
  return this->stopServer;
}

//
// stop the beerbot server
//
void BeerBotServer::stop() {
  Logger::log("stop beer bot server...");
  this->stopServer = true;
  close(this->serverSocket);
}

//
// default constructor
//
BeerBotServer::BeerBotServer(BeerBot* bot) {

  // save bot pointer
  this->bot = bot;

  // create server socket
  this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->serverSocket < 0) {
    Logger::log("ERROR opening server socket");
    return;
  }

  // init server address
  bzero((char *) &this->serv_addr, sizeof(this->serv_addr));
  serv_addr.sin_family      = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port        = htons(10101);

  // bind port
  if (bind(this->serverSocket, (struct sockaddr *) &this->serv_addr, sizeof(this->serv_addr)) < 0) {
    Logger::log("ERROR on binding server socket");
    return;
  }

  FD_ZERO(&this->clifds);
  FD_SET(this->serverSocket, &this->clifds);

}
