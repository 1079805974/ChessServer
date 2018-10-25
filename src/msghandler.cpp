#include "room.h"
#include "msghandler.h"
#include "utils.h"

Hub h;
static string initBoard =
    "rnbakabnr/9/1c5c1/p1p1p1p1p/9/9/P1P1P1P1P/1C5C1/9/RNBAKABNR w";

void onStart(vector<string> vs, WebSocket<SERVER> *ws)
{
  int roomIndex = atoi(vs[1].c_str());
  if (roomIndex > 0 && roomIndex < ROOMNUM)
  {
    if (!checkRoomAvalible(roomIndex))
    {
      sendMsg(ws, "error");
      return;
    }
    Room *rm = new Room();
    int index = getPlayerIndex(ws);
    Player *p = &players[index];
    p->room = rm;
    rm->hostIndex = index;
    rm->guestIndex = -1;
    rm->board = initBoard;
    rm->status = ROOM_READY;
    roomList[roomIndex] = rm;
  }
  boardcastStatus();
}

void onLogin(vector<string> vs, WebSocket<SERVER> *ws)
{
  int index;
  if ((index = getPlayerIndex(ws)) != -1)
  {
    renamePlayer(index, vs[1]);
  }
  else
  {
    Player *p = new Player();
    p->id = idCount++;
    p->nickname = vs[1].c_str();
    p->ws = ws;
    p->room = nullptr;
#ifdef DEBUG
    cout << "new Player Login" << vs[1] << endl;
#endif
    players.push_back(*p);
    playerCount++;
  }
}

void onStatus(vector<string> vs, WebSocket<SERVER> *ws)
{
  sendStatus(ws);
}

void sendStatus(WebSocket<SERVER> *ws)
{
  string result = "status ";
  string roomId = "0";
  for (int i = 0; i < ROOMNUM; i++)
  {
    roomId[0] = '0' + i;
    if (roomList[i] != nullptr)
    {
      sendMsg(ws, result + roomId + getPlayersNames(roomList[i]) + " " + roomList[i]->status);
    }
  }
}

void boardcastStatus()
{
  string result = "status ";
  string roomId = "0";
  string comGuestName;
  if(roomList[0]->guestIndex == -1){
    comGuestName = "null";
  }else{
    comGuestName = players[roomList[0]->guestIndex].nickname;
  }
  boardcast(result + roomId + " " + "Computer" + " " + comGuestName + " " + roomList[0] -> status);
  for (int i = 1; i < ROOMNUM; i++)
  {
    roomId[0] = '0' + i;
    if (roomList[i] != nullptr)
    {
      boardcast(result + roomId + getPlayersNames(roomList[i]) + " " + roomList[i]->status);
    }
  }
}

void onEnter(vector<string> vs, WebSocket<SERVER> *ws)
{
  int roomIndex = atoi(vs[1].c_str());
  if(roomIndex == 0){
    int index = getPlayerIndex(ws);
    Player *p = &players[index];
    p->room = roomList[0];
    roomList[0]->guestIndex = index;
    roomList[0]->status = ROOM_PLAYING;
  }
  if (roomIndex > 0 && roomIndex < ROOMNUM)
  {
    Room *rm = roomList[roomIndex];
    int index = getPlayerIndex(ws);
    Player *p = &players[index];
    p->room = rm;
    rm->guestIndex = index;
    rm->board = initBoard;
    rm->status = ROOM_PLAYING;
    rm->log.push(initBoard);
  }
  boardcastStatus();
}

void onMove(vector<string> vs, WebSocket<SERVER> *ws)
{
  string mv = vs[1];
  string fen = vs[2];
  string side = vs[3];
  int index = getPlayerIndex(ws);
  Player p = players[index];
  Room *rm = p.room;
  if(rm == roomList[0]){
    if(mv == "0"){
      
    }
  }else{
    if (index == rm->hostIndex)
      {
        rm->sendToGuest("move " + mv);
      }
      else if (index == rm->guestIndex)
      {
        rm->sendToHost("move " + mv);
      }
      rm->board = fen + " " + side;
      rm->log.push(rm->board);
  }
}

void sendMove(WebSocket<SERVER> *ws, string board)
{
  sendMsg(ws, "move " + board);
}
