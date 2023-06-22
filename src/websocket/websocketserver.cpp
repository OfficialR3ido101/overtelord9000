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
private:
    server m_endpoint;
};

void StartSocket() {
    BotWsServer s;
    s.run();
}
