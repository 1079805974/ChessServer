#pragma once
#include <iostream>
#include <stack>
#include <uWS/uWS.h>

#define ROOM_EMPTY "empty"
#define ROOM_READY "ready"
#define ROOM_PLAYING "playing"
#define ROOM_W_WIN "w_win"
#define ROOM_B_WIN "b_win"
#define ROOMNUM 6

using namespace std;
using namespace uWS;

typedef struct room Room;
typedef struct player Player;

struct room{
    int hostIndex;
    int guestIndex;
    
    enum turn {
        RED,
        BLACK
    };
    string status;
    string board;
    stack<string> log;

    void sendToHost(string msg);
    void sendToGuest(string msg);
};


struct player{
    int id;
    string nickname;
    WebSocket<SERVER> *ws;
    Room* room;
};


extern Room *roomList[ROOMNUM];
extern vector<Player> players;
extern int playerCount, idCount;

int getPlayerIndex(WebSocket<SERVER> *ws);
bool checkRoomAvalible(int index);
string getPlayersNames(const Room *rm);
void renamePlayer(int index, string nickname);