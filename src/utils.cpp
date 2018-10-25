#include "utils.h"
#include "room.h"
#include "msghandler.h"

void sendMsg(WebSocket<SERVER> *ws, string msg){
  cout << "send : " << msg << endl;
  ws->send(msg.data(), msg.length(), OpCode::TEXT);
}

void boardcast(string msg){
  h.uWS::Group<true>::broadcast(msg.data(), msg.length(), OpCode::TEXT);
}