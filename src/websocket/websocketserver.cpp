#include "websocketserver.h"

    void BotWsServer::run() {
        // Listen on port 9002
        m_endpoint.listen(std::stoi(getenv("WEBSOCKET_PORT")));

        // Queues a connection accept operation
        m_endpoint.start_accept();

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
    }
