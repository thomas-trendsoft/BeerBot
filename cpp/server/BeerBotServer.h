#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include "BeerBot.h"
#include "ClientHandler.h"

typedef struct handler_entry_str {
  ClientHandler* object;
  std::thread*   thread;
} handler_entry;

//
// server to communicate and receive commands
//
class BeerBotServer {

  // server socket descriptor
  int serverSocket;

  // bind server address
  struct sockaddr_in serv_addr;

  // client descriptor
  fd_set clifds;

  // client thread index
  int clientIdx;

  // server running flag
  int stopServer;

  BeerBot* bot;

  int handler_idx;

  handler_entry clients[10];

public:
  BeerBotServer(BeerBot* bot);

  int start();

  int isStopped();

  int acceptClient();

  void stop();

  void main_thread();

  void handle_client_thread(int cs);

};
