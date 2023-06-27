#include <Quotient/connection.h>
#include <Quotient/room.h>
#include <Quotient/qt_connection_util.h>
#include <qt5/QtCore/QCoreApplication>
#include <cstdlib>
#include <iostream>
#include <string>
#include <spdlog/spdlog.h>
#include <qt5/QtCore/QMetaObject>

void startMatrix(int argc, char* argv[], std::shared_ptr<spdlog::logger> log) {

    QCoreApplication app(argc, argv);
    const auto* userMxid = getenv("MATRIX_USERNAME");
    const auto* password = getenv("MATRIX_PASSWORD");
    const auto* deviceName = "AI";

    using Quotient::Connection;

    // You can create as many connections (to as many servers) as you need.
    // When logging in via mxid and password the connection gets a new access token.
    // Connection::connectWithToken() allows to connect using an existing access token.
    auto* c = new Connection(&app);
    c->loginWithPassword(userMxid, password, deviceName); // The homeserver is resolved from the user name
    app.connect(c, &Connection::connected, c, [c, &log] {
        qDebug() << "Connected, server: " << c->homeserver().toDisplayString();

        log->debug("[Matrix] Connected to: " + c->homeserver().toDisplayString().toStdString());
        c->syncLoop();
    });
    app.connect(c, &Connection::resolveError, c, [&](const QString& error) {

        log->debug("[Matrix] Failed to resolve server!");

        qDebug() << "Failed to resolve the server: " << error;
        app.exit(-2);
    });
    // connectSingleShot() is Qt's connect() that triggers exactly once and then disconnects
    Quotient::connectSingleShot(c, &Connection::syncDone, c, [c, &log] {
        const auto& allRooms = c->allRooms();
        qDebug() << "Sync done;" << allRooms.count() << "room(s) and"
                << c->users().count() << "user(s) received";
                std::string users = std::to_string(c->users().count());

        log->debug("Syncing users: " + users);

        std::cout << "sync";

        // Schedule logout after all rooms already queued for updating are done
        QMetaObject::invokeMethod(c, [c, &log] {
            qDebug() << "That's all, thank you";

            log->debug("[Matrix] That's all, thank you");

            c->logout();
        }, Qt::QueuedConnection);

        log->debug("[Matrix] Logout");
    });
    QObject::connect(c, &Connection::loadedRoomState, c, [c, &log](Quotient::Room* room) {
        qDebug() << "\nRoom display name:" << room->displayName()
                << "\nRoom topic:" << room->topic()
                << "\nJoined members:" << room->joinedCount() << "\n";

                log->debug("[Matrix] Connecting to rooms.");
    });
    QObject::connect(c, &Connection::loggedOut, &app, &QCoreApplication::quit);

    app.exec(); // Start event loop through which things actually happen
}
