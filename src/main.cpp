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
#include "utilities/utils.h"

void socketServer(BotWsServer *ws)
{
    ws->run();
}

void startDiscordBot(BotWsServer *ws)
{
    startDiscord(*ws);
}

int main(int argc, char *argv[])
{

    BotWsServer ws;
    MatrixBot mBot;
    BotUtils utils;

    // does check to see if config file exists and is valid.
    utils.checkConfigFile();

    std::thread websocket(socketServer, &ws);
    std::string expected_argument(argv[1]);

    qDebug() << expected_argument.c_str();

    if (expected_argument == "all")
    {

        std::thread discord(startDiscordBot, &ws);

        qInfo() << "[Main] "
                << "Press Ctrl+C To exit.";

        mBot.startMatrix(argc, argv, ws);
    }
    else if (expected_argument == "matrix")
    {

        qInfo() << "[Main] "
                << "Press Ctrl+C To exit.";

        mBot.startMatrix(argc, argv, ws);
    }
    else if (expected_argument == "irc")
    {
    }
    else if (argc != 1 || expected_argument != "all" || "matrix")
    {
        std::cout << "You need to tell me what to run by doing all or matrix"
                  << "\n"
                  << "The Websocket server will always be started!"
                  << "\n"
                  << "\n";
        return -1;
    }

    return 0;
}
