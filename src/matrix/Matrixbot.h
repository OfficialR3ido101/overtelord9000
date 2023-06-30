#pragma once

#include "../websocket/websocketserver.h"
#include <string>
#include <Quotient/room.h>

class MatrixBot {
public:
    int startMatrix(int argc, char* argv[], BotWsServer& ws);

private:
    void populateCommands();
    std::map<std::string, int> _commands;
    //check: We may need to delete rooms on exit.
    std::vector<Quotient::Room*> _rooms;
};
