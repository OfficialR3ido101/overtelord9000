#pragma once

#include <qjsonobject.h>
#include "websocket/websocketserver.h"


class BotUtils {
public:
    void addOvertePlayer(std::string revPlatform, std::string username, bool can_adjust_locks,
                         bool can_connect,
                         bool can_connect_past_max_capacity,
                         bool can_get_and_set_private_user_data,
                         bool can_kick,
                         bool can_replace_content,
                         bool can_rez,
                         bool can_rez_avatar_entities,
                         bool can_rez_tmp,
                         bool can_write_to_asset_server,
                         BotWsServer &ws);
    void setOvertePlayerPermissions(std::string revPlatform, std::string username, bool can_adjust_locks,
                         bool can_connect,
                         bool can_connect_past_max_capacity,
                         bool can_get_and_set_private_user_data,
                         bool can_kick,
                         bool can_replace_content,
                         bool can_rez,
                         bool can_rez_avatar_entities,
                         bool can_rez_tmp,
                         bool can_write_to_asset_server,
                         BotWsServer &ws);
    void removeOvertePlayer(std::string revPlatform, std::string username, BotWsServer &ws);
    void banOvertePlayer(std::string revPlatform, std::string username, BotWsServer &ws);
    void kickOvertePlayer(std::string revPlatform, std::string username,BotWsServer &ws);
    void restartDomainServer(std::string revPlatform, BotWsServer &ws);

private:

};
