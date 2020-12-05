#include "BeerBotServer.h"

//
// default constructor
// 
BeerBotServer::BeerBotServer() {
  // create server socket
  this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->serverSocket < 0)
    error("ERROR opening server socket");
  // init server address
  bzero((char *) &this->serv_addr, sizeof(this->serv_addr));
  serv_addr.sin_family      = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port        = htons(10101);

  // bind port
  if (bind(this->serverSocket, (struct sockaddr *) &this->serv_addr, sizeof(this->serv_addr)) < 0)
    error("ERROR on binding server socket");
}
