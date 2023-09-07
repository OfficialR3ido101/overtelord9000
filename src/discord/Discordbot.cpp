#include <iostream>
#include <dpp/dpp.h>
#include <dpp/channel.h>
#include <cstdlib>
#include "../websocket/websocketserver.h"
#include "../utilities/utils.h"
#include <iomanip>
#include <dpp/dpp.h>
#include <fmt/format.h>
#include <qt5/QtCore/QCoreApplication>
#include <QDebug>

void startDiscord(BotWsServer &ws) {

    QFile file("Config.json");
    QJsonDocument doc;
    QJsonObject obj;
    if(file.open(QIODevice::ReadOnly)) {
        doc = QJsonDocument::fromJson(file.readAll());
        obj = doc.object();
    }

    uint64_t channelid = std::stoull(obj["discord"]["welcome_channel"].toString().toStdString()]);

    dpp::cluster bot(obj["discord"]["token"].toString().toStdString()), dpp::i_default_intents | dpp::i_guild_members);

    BotUtils bu;

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
        if(dpp::run_once<struct register_bot_commands>()){
            dpp::slashcommand allowOverteAccess("allowlist","send message to webhook to allow a user to connect to server.", bot.me.id);
            dpp::slashcommand banOverteUser("overte_ban","Ban a user in your Overte domain.", bot.me.id);
            dpp::slashcommand modifyOverteUser("modify_user", "Use this to set user permissions if no user is already whitelisted nothing will happen.", bot.me.id);

            allowOverteAccess.add_option(
                dpp::command_option(dpp::co_string, "user", "the user to be allowlisted to overte server.", true)
            );

            banOverteUser.add_option(
                dpp::command_option(dpp::co_string, "user", "User needed to be banned.", true)
            );

            modifyOverteUser.add_option(
                dpp::command_option(dpp::co_string, "user", "User to be modified.", true)
            );

            bot.global_command_create(allowOverteAccess);
            bot.global_command_create(banOverteUser);
            qDebug() << "Commands are registered!";
        }
        qDebug() << "We are online as: " << bot.me.username.c_str();

        bot.set_presence(dpp::presence(dpp::ps_dnd, dpp::at_watching, "All the OwO!"));

    });

    bot.on_slashcommand([&bot, &ws, &bu](const dpp::slashcommand_t & event) {

        if(event.command.get_command_name() == "allowlist") {
            std::string username = std::get<std::string>(event.get_parameter("user"));

            bu.addOvertePlayer("discord", username, false, false, false ,false, false, false, false, false, false, false, ws);
            //ws.sendClientMessage("Hello");
            event.reply("Added!" + username);

        }
        if(event.command.get_command_name() == "overte_ban") {
            std::string username = std::get<std::string>(event.get_parameter("user"));

            bu.addOvertePlayer("discord", username, false, false, false ,false, false, false, false, false, false, false, ws);
            //ws.sendClientMessage("Hello");
            event.reply("Added!" + username);

        }
    });

    bot.on_interaction_create([&bot](const dpp::interaction_create_t & event){
        if(event.command.get_command_name() == "owo"){
            event.reply("OwO!");
        }
    });

    bot.start(dpp::st_wait);

};
