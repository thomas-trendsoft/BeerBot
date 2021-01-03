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
#include "Logger.h"


// long running tasks thread
void ClientHandler::task_thread() {

  while (currentTask() != STOP_TASKS) {
    switch (currentTask()) {
      case MAP_TASK:
        bot->exploreMap();
        break;
    }
    setTask(NO_TASK);
    delay(1000);
  }
  Logger::log("stop client long run thread");

}


//
// default constructor
//
ClientHandler::ClientHandler(int socket,BeerBot* bot) {
  this->socket       = socket;
  this->bot          = bot;
  this->error        = 0;
  this->long_task_id = NO_TASK;
}

// free memory msg
void ClientHandler::freeMsg(msgdata* msg) {

  msgdata* temp;
  msgdata* entry = msg;
  while (entry != NULL) {
    temp = entry->next;
    delete entry;
    entry = temp;
  }

}

//
// read basic message from protocol
//
msgdata* ClientHandler::readMessage() {
  vector<char>  buf(4096);
  int           rc;
  msgdata*      msg = NULL;

  // read first packet
  rc = recv(this->socket,buf.data(),buf.size(),0);
  if (rc < 6) {
    this->errmsg = "client read error ";
    this->error = -1;
    return msg;
  }

  Logger::log(string("read msg: ") + string(buf.data()));

  // tokenize
  char* save1;

  char* token = strtok_r(buf.data(),";",&save1);
  if (token == NULL) {
    this->errmsg = "failed to parse message";
    this->error = -1;
    return msg;
  }

  // parse msg len and read further bytes if needed
  token = strtok_r(save1,";",&save1);

  int mlen  = atoi(token);
  int limit = min(mlen,2048);

  int      key = 1;
  char*    kval;
  msgdata* last = NULL;

  while ((token = strtok_r(save1,";",&save1))) {

    if (key) {
      kval = token;
      key = 0;
    } else {
      msgdata* entry = new msgdata;

      entry->next  = NULL;
      entry->key   = string(kval);
      entry->value = string(token);

      cout << kval << " -> " << token << endl;
      if (last == NULL) {
        last = msg = entry;
      } else {
        last->next = entry;
        last = entry;
      }
      key = 1;
    }
  }

  return msg;
}

//
// basic message send to client for this handler
//
int ClientHandler::sendMessage(msgdata* msg) {
  int clen = 0;

  stringstream ss;
  msgdata* entry = msg;
  while (entry != NULL)
  {
    clen += entry->key.length() + 1;
    clen += entry->value.length() + 1;
    entry = entry->next;
  }

  // create output string
  stringstream data;

  data << "LEN;" << clen << ";" << ss.str();

  entry = msg;
  while (entry != NULL)
  {
    data << entry->key << ";";
    data << entry->value << ";";
    entry = entry->next;
  }

  data << "\n";

  string dstr = data.str();

  // send msg
  return send(this->socket,dstr.data(),dstr.length(),0);

}

// active long running task
int ClientHandler::currentTask() {
  return long_task_id;
}

// update running task
void ClientHandler::setTask(int id) {
  long_task_id = id;
}

//
// protocol handshake
//
int ClientHandler::handShake() {
    Logger::log("open client connection...");

    msgdata* hello = new msgdata;
    msgdata* msg   = this->readMessage();

    // check handshake message
    if (msg->key.compare("PROST") != 0) {
      freeMsg(msg);
      delete hello;
      shutdown();
      return -1;
    } else {
      hello->key = string("PROST");
      hello->value = string("1");

      Logger::log("send hello back");
      sendMessage(hello);
      freeMsg(msg);
      freeMsg(hello);
    }

    // open task thread for client
    std::thread( [this] { this->task_thread(); }).detach();

    return 0;
}


//
// client command handling
//
void ClientHandler::handle() {
  Logger::log("handle client...");

  // read and parse commands
  while (true) {

    // read client message
    msgdata* msg = this->readMessage();

    // check error on readMessag
    if (this->error < 0) {
      Logger::log("shutdown client close/error");
      break;
    }

    // check close connection
    if (msg->key.compare("CLOSE")==0) {
      this->freeMsg(msg);
      Logger::log("close client connection");
      break;
    }

    msgdata* resp = new msgdata;
    msgdata* last = resp;

    // handle pull info
    if (msg->key.compare("PULL")==0) {

      if (msg->value.compare("DIST")==0) {

        // measure current distance view
    	  double dist = bot->checkDistance();
    	  resp->key   = "RESULT";
        resp->value = std::to_string(dist);

      } else if (msg->value.compare("STAT")==0) {

        // send position and angle
        bb_position* spos = new bb_position;
        this->bot->currentPos(spos);

    	  resp->key   = "RESULT";
        resp->value = "OK";
        resp->next  = new msgdata;
        last = resp->next;

    	  last->key   = "X";
        last->value = std::to_string(spos->x);
        last->next  = new msgdata;
        last = last->next;

    	  last->key   = "Y";
        last->value = std::to_string(spos->y);
        last->next  = new msgdata;
        last = last->next;

    	  last->key   = "THETA";
        last->value = std::to_string(spos->theta);

        delete spos;

      } else if (msg->value.compare("MAP")==0) {
    	  stringstream ss;
        TinySLAM     slam = bot->getMap();

        // cut map area data to string
        for (int y=0;y<50;y++) {
          for (int x=0;x<50;x++) {
            ss << slam.map[y * TS_MAP_SIZE + x] << "/";
          }
        }

    	  resp->key   = "RESULT";
        resp->value = ss.str();
      } else {
    	  resp->key   = "RESULT";
        resp->value = "UNKOWN";
      }

    // handle command messages
  } else if (msg->key.compare("CMD")==0) {

      // check move command
      if (msg->value.compare("MOVE")==0) {
        this->moveCommand(msg->next);
        resp->key    = "RESULT";
        resp->value  = "OK";
      } else if (msg->value.compare("EYECAL")==0) {
        bot->eyeCalibration();
        resp->key    = "RESULT";
        resp->value  = "OK";
      } else if (msg->value.compare("STARTMAP")==0) {
        this->setTask(MAP_TASK);
        resp->key    = "RESULT";
        resp->value  = "OK";
      } else {
        resp->key   = "RESULT";
        resp->value = "unknown";
      }

    // unknown messages
    } else {
      resp->key   = "RESULT";
      resp->value = "unknown";
    }

    Logger::log("send answer");

    this->sendMessage(resp);

    // free memory
    this->freeMsg(msg);
    this->freeMsg(resp);
  }
}

// move command execution
void ClientHandler::moveCommand(msgdata* msg) {
  string mdir = msg->value;
  // TODO make fire and forget or threads
  if (mdir.compare("STOP")==0) {
    this->setTask(NO_TASK);
    this->bot->stop();
  } else if (mdir.compare("LEFT")==0) {
    this->bot->turnLeft();
  } else if (mdir.compare("RIGHT")==0) {
    this->bot->turnRight();
  } else if (mdir.compare("FORWARD")==0) {
    this->bot->forward();
  } else if (mdir.compare("BACKWARD")==0) {
    this->bot->backward();
  }
}


//
// close client connection
//
void ClientHandler::shutdown() {
  Logger::log("close client connection");
  this->long_task_id = STOP_TASKS;
  close(this->socket);
}
