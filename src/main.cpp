#include "discord/Discordbot.h"
#include "matrix/Matrixbot.h"
#include "websocket/websocketserver.h"
#include <iostream>
#include <string>
#include <thread>
#include <Quotient/connection.h>
#include <Quotient/room.h>
#include <Quotient/qt_connection_util.h>
#include <qt5/QtCore/QCoreApplication>
#include <qt5/QtCore/QMetaObject>

void socketServer() {
    BotWsServer ws;
    ws.run();
}

void startDiscordBot() {
    startDiscord();
}

int main(int argc, char* argv[]) {

    if(getenv("DISCORD_TOKEN") == nullptr
        || getenv("WELCOME_CHANNEL") == nullptr
        || getenv("WEBSOCKET_PORT") == nullptr
        || getenv("MATRIX_USERNAME") == nullptr
        || getenv("MATRIX_PASSWORD") == nullptr
        || getenv("MATRIX_PREFIX") == nullptr)
    {
        std::cout << "Enviroment variables are not set please check to see if you have set DISCORD_TOKEN, WELCOME_CHANNEL, MATRIX_USERNAME, MATRIX_PASSWORD, MATRIX_PREFIX and WEBSOCKET_PORT" << "\n" << "\n";
        return 1;
    }
    std::thread websocket (socketServer);
    std::thread discord (startDiscordBot);

    startMatrix(argc, argv);

    qInfo() << "Press Ctrl+C To exit.";
}
