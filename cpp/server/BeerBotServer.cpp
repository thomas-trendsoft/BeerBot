#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include "BeerBotServer.h"
#include "ClientHandler.h"

void *beerbotserver_main_thread(void* sptr) {
  BeerBotServer* server = (BeerBotServer*)sptr;

  std::cout << "start wait for client..." << std::endl;
  while (!server->isStopped()) {
    int cc = server->acceptClient();
    std::cout << "accepted client..." << cc << std::endl;
  }
  pthread_exit(NULL);
}

//
// open a new client connection
//
int BeerBotServer::acceptClient() {
  int* clientSocket = new int;
  struct timeval tv;

  struct sockaddr  client_addr;
  socklen_t        client_addr_size = sizeof(client_addr);

  tv.tv_sec = (long)5;
  tv.tv_usec = 0;

  if (((*clientSocket) = accept(this->serverSocket,(struct sockaddr *) &client_addr,&client_addr_size))<0)
  {
    perror("accept failed for client connection");
    return -1;
  }

  // check client socket state
  if (*clientSocket > 0) {
    if (pthread_create(&this->clientThreads[this->clientIdx],NULL,&client_handler_start,(void*)(clientSocket))) {
      perror("failed to start client thread");
      return -1;
    }

    this->clientIdx = (this->clientIdx + 1) % 10;
    return 1;
  } else if (*clientSocket < 0) {
    std::cout << "failed to select client socket" << std::endl;
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
    std::cout << "ERROR on listen to server port" << std::endl;
    return -1;
  }

  // server thread start
  if (pthread_create(&this->serverThread,NULL,&beerbotserver_main_thread,(void*)this)) {
    std::cout << "Unable to start server main thread." << std::endl;
    return -1;
  }

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
  std::cout << "stop beer bot server..." << std::endl;
  this->stopServer = true;
  close(this->serverSocket);
}

//
// default constructor
//
BeerBotServer::BeerBotServer() {
  // create server socket
  this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->serverSocket < 0) {
    std::cout << "ERROR opening server socket" << std::endl;
    return;
  }

  // init server address
  bzero((char *) &this->serv_addr, sizeof(this->serv_addr));
  serv_addr.sin_family      = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port        = htons(10101);

  // bind port
  if (bind(this->serverSocket, (struct sockaddr *) &this->serv_addr, sizeof(this->serv_addr)) < 0) {
    std::cout << "ERROR on binding server socket" << std::endl;
    return;
  }

  FD_ZERO(&this->clifds);
  FD_SET(this->serverSocket, &this->clifds);

}
