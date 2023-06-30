#include "Matrixbot.h"

#include <Quotient/connection.h>
#include <Quotient/room.h>
#include <Quotient/qt_connection_util.h>

#include <qt5/QtCore/QCoreApplication>
#include <qt5/QtCore/QMetaObject>


#include <cstdlib>
#include <iostream>
#include <string>

#include "../websocket/websocketserver.h"

int MatrixBot::startMatrix(int argc, char* argv[], BotWsServer& ws) {

    QCoreApplication app(argc, argv);
    const QString userMxid = getenv("MATRIX_USERNAME");
    const QString password = getenv("MATRIX_PASSWORD");
    const QString deviceName = "AI";

    using Quotient::Connection;
    using Quotient::Room;

    auto* c = new Connection(&app);



    c->loginWithPassword(userMxid, password, deviceName);
    app.connect(c, &Connection::connected, c, [c] {
        qDebug() << "Connected, server: " << c->homeserver().toDisplayString();

        c->syncLoop();
    });
    app.connect(c, &Connection::resolveError, c, [&](const QString& error) {

        qDebug() << "[Matrix] " << "Failed to resolve the server: " << error;
        app.exit(-2);
    });

    Quotient::connectSingleShot(c, &Connection::syncDone, c, [c] {
        const auto& allRooms = c->allRooms();
        qDebug() << "[Matrix] " << "Sync done;" << allRooms.count() << "room(s) and"
        << c->users().count() << "user(s) received";
        std::string users = std::to_string(c->users().count());

        QMetaObject::invokeMethod(c, [c] {
            qDebug() << "[Matrix] " << "That's all, thank you";

            //c->logout();
        }, Qt::QueuedConnection);
    });

    QObject::connect(c, &Connection::loadedRoomState, c, [c, this, &ws](Quotient::Room* room) {
        qDebug() << "[Matrix] " << "\nRoom display name:" << room->displayName()
        << "\nRoom topic:" << room->topic()
        << "\nJoined members:" << room->joinedCount() << "\n";
        this->_rooms.push_back(room);

        QObject::connect(room, &Room::addedMessages, room, [room, this, &ws] (int startIndex, int endIndex) {
            qInfo() << "New Messages added!" << "\n" << "\n" << "startIndex = " << startIndex << "\n" << " endIndex = " << endIndex;
            auto &timeline(room->messageEvents());


            for (int i = startIndex; i <= endIndex; i++) {
                auto contentJson (timeline[i].get()->contentJson());
                qInfo() << contentJson;
                QString cmdValue = contentJson.find("body").value().toString();
                //todo: replace with hashmap here.
                if(cmdValue == "/oof") {
                    auto message (room->postPlainText("sent a websocket message!"));
                    ws.sendClientMessage("Hello from matrix!");
                }
            }


        });

    });

    QObject::connect(c, &Connection::loggedOut, c, [&]() {
        qDebug() << "[Matrix] " << "We have been loggedOut.";
    });

    QObject::connect(&ws, &BotWsServer::finished, &app, &QCoreApplication::quit);

    int appReturnVal = app.exec();
    delete c;

    return appReturnVal;
}
