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

// free memory msg
void ClientHandler::freeMsg(map<string,msgdata*>* msg) {
  map<string,msgdata*>::iterator it;

  for (it = msg->begin();it != msg->end();++it) {
    delete it->second->key;
    delete it->second->value;
    delete it->second;
  }
  delete msg;
}

//
// read basic message from protocol
//
map<string,msgdata*>* ClientHandler::readMessage() {
  vector<char>        buf(4096);
  int                 rc;
  map<string,msgdata*>* msg = new map<string,msgdata*>;

  // read first packet
  rc = recv(this->socket,buf.data(),buf.size(),0);
  if (rc < 6) {
    std::cout << "err read: " << rc << std::endl;
    this->errmsg = "client read error ";
    this->error = -1;
    return msg;
  }

  std::cout << "read msg: " << buf.data() << std::endl;

  // tokenize
  char* save1;

  char* token = strtok_r(buf.data(),";",&save1);
  if (token == NULL) {
    this->errmsg = "failed to parse message";
    this->error = -1;
    return msg;
  }

  // parse msg len and read further bytes if needed
  std::cout << "len msg: " << token << std::endl;

  token = strtok_r(save1,";",&save1);
  std::cout << "len data: " << token << std::endl;

  int mlen = atoi(token);
  int limit = min(mlen,2048);

  cout << "parsed: " << mlen << "/" << limit << endl;

  int key = 1;
  string* kval;
  while ((token = strtok_r(save1,";",&save1))) {
    cout << "token: " << token << endl;
    if (key) {
      kval = new string(token);
      key = 0;
    } else {
      msgdata* entry = new msgdata;
      entry->key = kval;
      entry->value = new string(token);
      msg->insert(pair<string,msgdata*>(*kval,entry));
      cout << *kval << " -> " << token << endl;
      key = 1;
    }
  }

  return msg;
}

//
// basic message send to client for this handler
//
int ClientHandler::sendMessage(map<string,string> msg) {
  int clen = 0;

  map<string, string>::iterator it;

  stringstream ss;
  for (it = msg.begin(); it != msg.end(); it++)
  {
    clen += it->first.length() + 1;
    clen += it->second.length() + 1;
  }

  // create output string
  stringstream data;

  data << "LEN;" << clen << ";" << ss.str();

  for (it = msg.begin(); it != msg.end(); it++)
  {
    data << it->first << ";";
    data << it->second << ";";
  }

  data << "\n";

  string dstr = data.str();

  cout << "send client msg: " << dstr << endl;
  // send msg
  return send(this->socket,dstr.data(),dstr.length(),0);

}

//
// protocol handshake
//
int ClientHandler::handShake() {
    cout << "open client connection..." << std::endl;

    map<string,string>  hello;
    map<string,msgdata*>* msg = this->readMessage();

    // check handshake message
    if (msg->count("PROST") == 0) {
      cout << "No prost on hand shake!" << endl;
      this->shutdown();
      this->freeMsg(msg);
      return -1;
    } else {
      cout << "GOT HANDSHAKE" << endl;
      hello.insert(pair<string,string>("PROST","1"));
      this->sendMessage(hello);
      this->freeMsg(msg);
    }

    return 0;
}

//
// client command handling
//
void ClientHandler::handle() {
  std::cout << "handle client..." << std::endl;

  // read and parse commands
  while (true) {

    // read client message
    map<string,msgdata*>* msg = this->readMessage();

    // check error on readMessag
    if (this->error < 0) {
      cout << "shutdown client close/error" << endl;
      break;
    }

    // check close connection
    if (msg->count("CLOSE")==1) {
      this->freeMsg(msg);
      cout << "close client connection" << endl;
      break;
    }

    // handle pull info
    if (msg->count("PULL")==1) {
      map<string,string> resp;

      resp.insert(pair<string,string>("INFO","FAKE"));
      this->sendMessage(resp);
    // handle command messages
    } else if (msg->count("CMD")==1) {
      map<string,string> resp;

      resp.insert(pair<string,string>("RESULT","FAKE"));
      this->sendMessage(resp);
    }

    // free memory
    this->freeMsg(msg);
  }
}

//
// close client connection
//
void ClientHandler::shutdown() {
  std::cout << "close client connection" << std::endl;
  close(this->socket);
}
