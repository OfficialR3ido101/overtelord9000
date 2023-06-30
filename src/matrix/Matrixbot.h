#pragma once

#include "../websocket/websocketserver.h"
#include <string>
#include <Quotient/room.h>

class MatrixBot {
public:
    int startMatrix(int argc, char* argv[], BotWsServer& ws);

private:
    std::vector<std::string> _commands;
    // We may need to delete rooms on exit.
    std::vector<Quotient::Room*> _rooms;


};
