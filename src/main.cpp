#include "discord/bot.h"
#include "websocket/websocketserver.h"

BotWsServer websocketServer;

void socketServer() {
    websocketServer.run();
}

void startDiscordBot() {
    startDiscord();
}


int main(int argc, char* argv[]) {

    std::thread websocket (socketServer);
    std::thread discord (startDiscordBot);

    do
    {
        std::cout << '\n' << "Press a key to exit..." << "\n" << "\n";
    } while (std::cin.get() != '\n');

    return 0;
}
