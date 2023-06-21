#include <iostream>
#include <dpp/dpp.h>
#include <dpp/channel.h>
#include <cstdlib>

int main(int argc, char **argv) {

    dpp::cluster bot(getenv("DISCORD_TOKEN"));
    char *channelend;
    ulong channelid = strtoul(getenv("WELCOME_CHANNEL"), &channelend, 10);


    bot.on_ready([&bot](const dpp::ready_t & event) {
    bot.set_presence(dpp::presence(dpp::ps_dnd, dpp::at_watching, "All the OwO!"));
    //todo: register all of the commands needed by the bot :)
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

    bot.on_interaction_create([&bot](const dpp::interaction_create_t & event){
        //todo: do funny command reply here :D
    });

    bot.start(dpp::st_wait);
    return 0;
}
