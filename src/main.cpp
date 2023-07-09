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

void socketServer(BotWsServer *ws) {
    ws->run();
}

void startDiscordBot(BotWsServer *ws) {
    startDiscord(*ws);
}

int main(int argc, char* argv[]) {

    if(argc != 2 || argv == nullptr){
        std::cout << "You need to tell me what to run by doing all or matrix"
        << "\n" << "The Websocket server will always be started!" << "\n" << "\n";
        return -1;
    }

    BotWsServer ws;
    MatrixBot mBot;

    if(getenv("WEBSOCKET_PORT") == nullptr) {
        std::cout << "You need to tell me what port to run on. please set WEBSOCKET_PORT enviroment variable!";
        return -1;
    }

    std::thread websocket (socketServer, &ws);
    std::string expected_argument(argv[1]);


    qDebug() << expected_argument.c_str();

    if(expected_argument == "all") {

        if(getenv("DISCORD_TOKEN") == nullptr
            || getenv("WELCOME_CHANNEL") == nullptr
            || getenv("MATRIX_USERNAME") == nullptr
            || getenv("MATRIX_PASSWORD") == nullptr
            || getenv("MATRIX_PREFIX") == nullptr)
        {
            std::cout << "Enviroment variables are not set please check to see if you have set DISCORD_TOKEN, WELCOME_CHANNEL, MATRIX_USERNAME, MATRIX_PASSWORD and MATRIX_PREFIX" << "\n" << "\n";
            return 1;
        }
        std::thread discord (startDiscordBot, &ws);

        qInfo() << "[Main] " << "Press Ctrl+C To exit.";

        mBot.startMatrix(argc, argv, ws);
    } else if (expected_argument == "matrix") {

        if(getenv("MATRIX_USERNAME") == nullptr
            || getenv("MATRIX_PASSWORD") == nullptr
            || getenv("MATRIX_PREFIX") == nullptr){
            std::cout << "Enviroment variables are not set please check to see if you have set MATRIX_USERNAME, MATRIX_PASSWORD and MATRIX_PREFIX" << "\n" << "\n";
            }

            qInfo() << "[Main] " << "Press Ctrl+C To exit.";

        mBot.startMatrix(argc, argv, ws);
    } else if(argc != 1 || expected_argument != "all" || "matrix"){
        std::cout << "You need to tell me what to run by doing all or matrix"
        << "\n" << "The Websocket server will always be started!" << "\n" << "\n";
        return -1;
    }

    return 0;
}
