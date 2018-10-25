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
#include "utils.h"

using namespace uWS;
using namespace std;

Room *roomList[ROOMNUM] = {nullptr};
vector<Player> players;
int playerCount = 0, idCount = 0;

void Room::sendToHost(string msg){
  sendMsg(players[hostIndex].ws, msg);
}

void Room::sendToGuest(string msg){
  sendMsg(players[guestIndex].ws, msg);
}


int getPlayerIndex(WebSocket<SERVER> *ws)
{
  for (int i = 0; i < players.size(); i++)
  {
    if (players[i].ws == ws)
      return i;
  }
  return -1;
}

void renamePlayer(int index, string nickname)
{
  players[index].nickname = nickname;
}

bool checkRoomAvalible(int index)
{
  if (roomList[index] == nullptr)
    return true;

  if (roomList[index]->status == ROOM_EMPTY)
    return true;
  return false;
}

string getPlayersNames(const Room *rm)
{
  if (rm->hostIndex == -1)
  {
    return " null null";
  }
  else if (rm->guestIndex == -1)
  {
    return " " + players[rm->hostIndex].nickname + " null";
  }
  else
  {
    return " " + players[rm->hostIndex].nickname + " " + players[rm->guestIndex].nickname;
  }
}
