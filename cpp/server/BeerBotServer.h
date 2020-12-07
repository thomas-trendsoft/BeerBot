#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

//
// server to communicate and receive commands
//
class BeerBotServer {

  // server socket descriptor
  int serverSocket;

  // bind server address
  struct sockaddr_in serv_addr;

  // main server thread
  pthread_t serverThread;

  // client threads
  pthread_t clientThreads[10];

  // client descriptor
  fd_set clifds;

  // client thread index
  int clientIdx;

  // server running flag
  int stopServer;

public:
  BeerBotServer();

  int start();

  int isStopped();

  int acceptClient();

  void stop();


};
