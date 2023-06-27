#include "discord/Discordbot.h"
#include "matrix/Matrixbot.h"
#include "websocket/websocketserver.h"
#include <iostream>
#include <string>
#include <thread>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

void socketServer() {
    BotWsServer ws;
    ws.run();
}

void startDiscordBot(std::shared_ptr<spdlog::logger> log) {
    startDiscord(log);
}

//void startMatrixBot(int argc, char* argv[]) {
//    startMatrix(argc, argv);
//}


int main(int argc, char* argv[]) {

    std::shared_ptr<spdlog::logger> log;
    const std::string log_name = "overtelord9000.log";
    spdlog::init_thread_pool(8192, 2);
    std::vector<spdlog::sink_ptr> sinks;
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
    auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_name, 1024 * 1024 * 5, 10);
    sinks.push_back(stdout_sink);
    sinks.push_back(rotating);
    log = std::make_shared<spdlog::async_logger>("logs", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    spdlog::register_logger(log);
    log->set_pattern("%^%Y-%m-%d %H:%M:%S.%e [%L] [th#%t]%$ : %v");
    log->set_level(spdlog::level::level_enum::debug);


    if(getenv("DISCORD_TOKEN") == nullptr
        || getenv("WELCOME_CHANNEL") == nullptr
        || getenv("WEBSOCKET_PORT") == nullptr
        || getenv("MATRIX_USERNAME") == nullptr
        || getenv("MATRIX_PASSWORD") == nullptr)
    {
        std::cout << "Enviroment variables are not set please check to see if you have set DISCORD_TOKEN, WELCOME_CHANNEL, MATRIX_USERNAME, MATRIX_PASSWORD and WEBSOCKET_PORT" << "\n";
        return 1;
    }

    std::thread websocket (socketServer);
    std::thread discord (startDiscordBot, log);

    startMatrix(argc, argv, log);

    do
    {
        std::cout << '\n' << "Press ENTER/RETURN to exit..." << "\n" << "\n";
    } while (std::cin.get() != '\n');

    return 0;
}
