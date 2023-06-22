#include <iostream>
#include <dpp/dpp.h>
#include <dpp/channel.h>
#include <cstdlib>
#include "websocket/websocketserver.h"
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <iomanip>
#include <dpp/dpp.h>
#include <fmt/format.h>

int main(int argc, char **argv) {

    if(getenv("DISCORD_TOKEN") == nullptr || getenv("WEBSOCKET_PORT") == nullptr) {
        std::cout << "Enviroment variables are not set please check to see if you have set DISCORD_TOKEN and WEBSOCKET_PORT";
        return 1;
    }

    dpp::cluster bot(getenv("DISCORD_TOKEN"));

    const std::string log_name = "overtelord9000.log";

    /* Set up spdlog logger */
    std::shared_ptr<spdlog::logger> log;
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

    bot.on_log([&bot, &log](const dpp::log_t & event) {
        switch (event.severity) {
            case dpp::ll_trace:
                log->trace("{}", event.message);
            break;
            case dpp::ll_debug:
                log->debug("{}", event.message);
            break;
            case dpp::ll_info:
                log->info("{}", event.message);
            break;
            case dpp::ll_warning:
                log->warn("{}", event.message);
            break;
            case dpp::ll_error:
                log->error("{}", event.message);
            break;
            case dpp::ll_critical:
            default:
                log->critical("{}", event.message);
            break;
        }
    });

    bot.on_ready([&bot](const dpp::ready_t & event) {
        std::cout << "We are online!" << bot.me.username;

        bot.set_presence(dpp::presence(dpp::ps_dnd, dpp::at_watching, "All the OwO!"));

        dpp::slashcommand owo("owo", "Sup", bot.me.id);
        bot.global_command_create(owo);

    });

    bot.on_interaction_create([&bot](const dpp::interaction_create_t & event){
        if(event.command.get_command_name() == "owo"){
            event.reply("OwO!");
        }
    });

    bot.start(dpp::st_wait);
    StartSocket();

    return 0;
}
