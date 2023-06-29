#include <iostream>
#include <dpp/dpp.h>
#include <dpp/channel.h>
#include <cstdlib>
#include "../websocket/websocketserver.h"
#include <iomanip>
#include <dpp/dpp.h>
#include <fmt/format.h>
#include <qt5/QtCore/QCoreApplication>
#include <qlogging.h>
#include <QDebug>

void startDiscord() {

    uint64_t channelid = std::stoull(getenv("WELCOME_CHANNEL"));

    dpp::cluster bot(getenv("DISCORD_TOKEN"), dpp::i_default_intents | dpp::i_guild_members);

    const std::string log_name = "overtelord9000.log";

    BotWsServer ws;

    bot.on_log([&bot](const dpp::log_t & event) {
        switch (event.severity) {
            case dpp::ll_trace:
                qDebug() << "[Discord] " << event.message.c_str();
                break;
            case dpp::ll_debug:
                qDebug() << "[Discord] " << event.message.c_str();
                break;
            case dpp::ll_info:
                qInfo() << "[Discord] " << event.message.c_str();
                break;
            case dpp::ll_warning:
                qWarning() << "[Discord] " << event.message.c_str();
                break;
            case dpp::ll_error:
                qCritical() << "[Discord] " << event.message.c_str();
                break;
            case dpp::ll_critical:
            default:
                qCritical() << "[Discord] " << event.message.c_str();
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

    bot.on_ready([&bot](const dpp::ready_t & event) {
        qDebug() << "We are online as: " << bot.me.username.c_str();

        bot.set_presence(dpp::presence(dpp::ps_dnd, dpp::at_watching, "All the OwO!"));

        dpp::slashcommand sendOverte("overtemessage","send message to webhook.", bot.me.id);

        sendOverte.add_option(
            dpp::command_option(dpp::co_string, "message", "message to be sent", true)
        );

        bot.global_command_create(sendOverte);

        qDebug() << "Commands are registered!";

    });

    bot.on_slashcommand([&bot, &ws](const dpp::slashcommand_t & event) {
        if(event.command.get_command_name() == "overtemessage") {
            std::string message = std::get<std::string>(event.get_parameter("message"));

            ws.sendClientMessage(message);
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
