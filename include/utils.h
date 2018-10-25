#pragma once
#include <iostream>
#include <stack>
#include <uWS/uWS.h>

using namespace std;
using namespace uWS;

void sendMsg(WebSocket<SERVER> *ws, string msg);
void boardcast(string msg);