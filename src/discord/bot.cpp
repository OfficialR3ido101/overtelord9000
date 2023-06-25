#include <iostream>
#include <dpp/dpp.h>
#include <dpp/channel.h>
#include <cstdlib>
#include "../websocket/websocketserver.h"
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <iomanip>
#include <dpp/dpp.h>
#include <fmt/format.h>

void startDiscord() {

    if(getenv("DISCORD_TOKEN") == nullptr || getenv("WELCOME_CHANNEL") == nullptr || getenv("WEBSOCKET_PORT") == nullptr) {
        std::cout << "Enviroment variables are not set please check to see if you have set DISCORD_TOKEN, WELCOME_CHANNEL and WEBSOCKET_PORT";
        return;
    }

    uint64_t channelid = std::stoull(getenv("WELCOME_CHANNEL"));

    dpp::cluster bot(getenv("DISCORD_TOKEN"), dpp::i_default_intents | dpp::i_guild_members);

    const std::string log_name = "overtelord9000.log";

    BotWsServer websocketServer;

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

    bot.on_guild_member_add([&bot, channelid](const dpp::guild_member_add_t & event){
        dpp::embed embed = dpp::embed()
        .set_color(dpp::colors::green)
        .set_title("New User!")
        .add_field(
            "User Joined",
            event.added.get_user()->username
        )
        .set_image(event.added.get_avatar_url())
        .set_timestamp(time(0));

        bot.message_create(dpp::message(channelid, embed));
    });

    bot.on_guild_member_remove([&bot, channelid](const dpp::guild_member_remove_t & event){
        dpp::embed embed = dpp::embed()
        .set_color(dpp::colors::scarlet_red)
        .set_title("User Left!")
        .add_field(
            "User Left",
            event.removed->username
        )
        .set_image(event.removed->get_avatar_url())
        .set_timestamp(time(0));

        bot.message_create(dpp::message(channelid, embed));
    });

    bot.on_ready([&bot, &log](const dpp::ready_t & event) {
        log->info("We are online as: " + bot.me.username);

        bot.set_presence(dpp::presence(dpp::ps_dnd, dpp::at_watching, "All the OwO!"));

        dpp::slashcommand sendOverte("overtemessage","send message to webhook.", bot.me.id);

        sendOverte.add_option(
            dpp::command_option(dpp::co_string, "message", "message to be sent", true)
        );

        bot.global_command_create(sendOverte);

        log->info("Commands are registered!");

    });

    bot.on_slashcommand([&bot, &websocketServer](const dpp::slashcommand_t & event) {
        if(event.command.get_command_name() == "overtemessage") {
            std::string message = std::get<std::string>(event.get_parameter("message"));

            websocketServer.sendClientMessage(message);
            event.reply(std::string("sent message: " + message));
        }
    });

    bot.on_interaction_create([&bot](const dpp::interaction_create_t & event){
        if(event.command.get_command_name() == "owo"){
            event.reply("OwO!");
        }
    });

    bot.start(dpp::st_wait);

};
