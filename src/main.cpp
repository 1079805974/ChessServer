#include <sys/stat.h>
#include <sys/types.h>
#include <uWS/uWS.h>
#include <unistd.h>
#include <stdlib.h>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>

#include "room.h"
#include "msghandler.h"
#include "httphandler.h"

using namespace uWS;
using namespace std;

vector<string> split(const string &s, const char flag = ' ');

int main()
{
  static int count = 0;
  Player *p = new Player();
  p->id = 0;
  p->nickname = "Computer";
  p->ws = NULL;
  players.push_back(*p);
  Room *computerRoom = new Room();
  roomList[0] = computerRoom;
  p->room = computerRoom;
  computerRoom->hostIndex = 0;
  computerRoom->guestIndex = -1;
  computerRoom->status = ROOM_READY;
  h.onDisconnection(
      [](WebSocket<SERVER> *ws, int code, char *message, size_t length) {
        int index = getPlayerIndex(ws);
        if (index != -1)
          if (players[index].room != nullptr)
          {
            if (players[index].room->hostIndex == index)
            {
              if(players[index].room->guestIndex == -1){
                players[index].room->status = ROOM_EMPTY;
              }else{
                players[index].room->status = ROOM_READY;
              }
              players[index].room->hostIndex = players[index].room->guestIndex;
              players[index].room->guestIndex = -1;
            }
            else
            {
              players[index].room->guestIndex = -1;
              players[index].room->status = ROOM_READY;
            }
            
            players[index].room = nullptr;
            boardcastStatus();
          }
      });

  h.onConnection([](WebSocket<SERVER> *ws, HttpRequest req) {
    std::cout << "a client coming" << std::endl;
  });

  h.onMessage(
      [](WebSocket<SERVER> *ws, char *message, size_t length, OpCode opCode) {
        string msg = message;
        msg = msg.substr(0, length);
        vector<string> msgArr = split(msg);
        auto it = dispatch.find(msgArr[0]);
        if (it != dispatch.end() && (msgArr.size() > 1))
        {
          (*(it->second))(msgArr, ws);
        }
        std::cout << msg << std::endl;
      });

  h.onHttpRequest(httpHandler);

  if (h.listen(3000))
  {
    h.run();
  }
}

vector<string> split(const string &s, const char flag)
{
  vector<string> *ss = new vector<string>();
  vector<string> &sv = *ss;
  sv.clear();
  istringstream iss(s);
  string temp;

  while (getline(iss, temp, flag))
  {
    sv.push_back(temp);
  }
  return sv;
}