#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

//
// server to communicate and receive commands
//
class BeerBotServer {

  int serverSocket;

  struct sockaddr_in serv_addr;

public:
  BeerBotServer();

};
