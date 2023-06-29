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

//void startMatrixBot(int argc, char* argv[]) {
//    startMatrix(argc, argv);
//}


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

    QCoreApplication app(argc, argv);
    const auto* userMxid = "@animatedmanaquinn:overte.org";
    const auto* password = "Tc4XegwNcu4DU0ZL2zOG";
    const auto* deviceName = "AI";

    using Quotient::Connection;

    // You can create as many connections (to as many servers) as you need.
    // When logging in via mxid and password the connection gets a new access token.
    // Connection::connectWithToken() allows to connect using an existing access token.
    auto* c = new Connection(&app);
    c->loginWithPassword(userMxid, password, deviceName); // The homeserver is resolved from the user name
    app.connect(c, &Connection::connected, c, [c] {
        qDebug() << "Connected, server: " << c->homeserver().toDisplayString();

        c->syncLoop();
    });
    app.connect(c, &Connection::resolveError, c, [&](const QString& error) {

        qDebug() << "Failed to resolve the server: " << error;
        app.exit(-2);
    });
    // connectSingleShot() is Qt's connect() that triggers exactly once and then disconnects
    Quotient::connectSingleShot(c, &Connection::syncDone, c, [c] {
        const auto& allRooms = c->allRooms();
        qDebug() << "Sync done;" << allRooms.count() << "room(s) and"
                << c->users().count() << "user(s) received";
                std::string users = std::to_string(c->users().count());

        std::cout << "sync";

        // Schedule logout after all rooms already queued for updating are done
        QMetaObject::invokeMethod(c, [c] {
            qDebug() << "That's all, thank you";

            //c->logout();
        }, Qt::QueuedConnection);
    });
    QObject::connect(c, &Connection::loadedRoomState, c, [c](Quotient::Room* room) {
        qDebug() << "\nRoom display name:" << room->displayName()
                << "\nRoom topic:" << room->topic()
                << "\nJoined members:" << room->joinedCount() << "\n";
    });


    //QObject::connect(c, &Connection::loggedOut, &app, &QCoreApplication::quit);

        QObject::connect(c, &Connection::loggedOut, c, [&]() {
        app.exit(-2);
    });

    std::thread websocket (socketServer);
    std::thread discord (startDiscordBot);

    qInfo() << "Press Ctrl+C To exit.";

    return app.exec();
}
