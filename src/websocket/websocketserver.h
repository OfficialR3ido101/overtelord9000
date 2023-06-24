#pragma once

#include <set>

#include <websocketpp/config/asio.hpp>
#include <websocketpp/server.hpp>

#include <functional>
#include <websocketpp/common/connection_hdl.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::lib::placeholders::_1;

class BotWsServer {
public:
    BotWsServer() {
        // Set logging settings
        m_endpoint.set_error_channels(websocketpp::log::elevel::all);
        m_endpoint.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);

        using websocketpp::connection_hdl;

        // Initialize Asio
        m_endpoint.init_asio();
        m_endpoint.set_open_handler(bind(&BotWsServer::on_open,this,::_1));
        m_endpoint.set_close_handler(bind(&BotWsServer::on_close,this,::_1));
    }

    void on_open(websocketpp::connection_hdl hdl) {
        m_connections.insert(hdl);
    }

    void on_close(websocketpp::connection_hdl hdl) {
        m_connections.erase(hdl);
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
    typedef std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl>> con_list;
    con_list m_connections;

    typedef server::message_ptr message_ptr;

    void sendMessage(std::string msg) {

        std::cout << "on_message called with hdl: "
        << " and message: " << msg
        << std::endl;

        for(auto connection : m_connections) {
            try {

                m_endpoint.send(connection, msg, websocketpp::frame::opcode::text);
            } catch (websocketpp::exception const & e) {

                std::cout << "Echo failed because: "
                << "(" << e.what() << ")" << std::endl;

            }
        }

    }
};
