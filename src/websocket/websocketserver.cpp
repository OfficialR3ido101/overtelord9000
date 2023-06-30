#include "websocketserver.h"
#include <qlogging.h>
#include <QDebug>

void BotWsServer::run() {

    m_endpoint.listen(std::stoi(getenv("WEBSOCKET_PORT")));

    // Queues a connection accept operation
    m_endpoint.start_accept();

    qDebug() << "[Websocket]" << "Websocket server starting!";

    // Start the Asio io_service run loop
    m_endpoint.run();
}

void BotWsServer::sendClientMessage (std::string message) {
    sendMessage(message);
}

void BotWsServer::on_open(websocketpp::connection_hdl hdl) {
    m_connections.insert(hdl);
}

void BotWsServer::on_close(websocketpp::connection_hdl hdl) {
    m_connections.erase(hdl);
    qInfo() << "[Websocket] " << "Closed Websocket server!";
    emit finished();
}

void BotWsServer::on_message(websocketpp::connection_hdl hdl) {
    m_connections.insert(hdl);
}
