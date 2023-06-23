#pragma once

#include <websocketpp/config/asio.hpp>
#include <websocketpp/server.hpp>

#include <functional>

typedef websocketpp::server<websocketpp::config::asio> server;

class BotWsServer {
public:
    BotWsServer() {
        // Set logging settings
        m_endpoint.set_error_channels(websocketpp::log::elevel::all);
        m_endpoint.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        m_endpoint.init_asio();
    }

    void run() {
        // Listen on port 9002
        m_endpoint.listen(std::stoi(getenv("WEBSOCKET_PORT")));

        // Queues a connection accept operation
        m_endpoint.start_accept();

        // Start the Asio io_service run loop
        m_endpoint.run();
    }

    void sendClientMessage(std::string message) {
        sendMessage(message);
    }
private:
    server m_endpoint;

    typedef server::message_ptr message_ptr;

    void sendMessage(std::string msg) {
        websocketpp::connection_hdl hdl;

        std::cout << "on_message called with hdl: " << hdl.lock().get()
        << " and message: " << msg
        << std::endl;

        try {
            m_endpoint.send(hdl, msg, websocketpp::frame::opcode::text);
        } catch (websocketpp::exception const & e) {
            std::cout << "Echo failed because: "
            << "(" << e.what() << ")" << std::endl;
        }

    }
};
