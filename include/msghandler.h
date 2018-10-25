#pragma once

#include <uWS/uWS.h>
#include <stdlib.h>
#include <iostream>
#include <map>

#include "room.h"
#include "msghandler.h"


using namespace uWS;
using namespace std;

extern Hub h;

typedef void (*FP)(vector<string>, WebSocket<SERVER> *);

void initHandler();

void onStart(vector<string> vs, WebSocket<SERVER> *ws);

void onLogin(vector<string> vs, WebSocket<SERVER> *ws);

void onStatus(vector<string> vs, WebSocket<SERVER> *ws);

void onEnter(vector<string> vs, WebSocket<SERVER> *ws);

void onMove(vector<string> vs, WebSocket<SERVER> *ws);

void sendMove(WebSocket<SERVER> *ws, string board);

void sendStatus(WebSocket<SERVER> *ws);


void boardcastStatus();

static map<string, FP> dispatch {
     {"login", onLogin},  // login nickname
     {"start", onStart},  // start roomID
     {"enter", onEnter}, // enter roomID
     {"move", onMove},    //move fen side
     {"status", onStatus}, //status get
};
