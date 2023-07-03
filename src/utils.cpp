#include <QJsonObject>
#include "websocket/websocketserver.h"
#include "utils.h"

void BotUtils::addOvertePlayer(std::string username, bool can_adjust_locks,
                         bool can_connect,
                         bool can_connect_past_max_capacity,
                         bool can_get_and_set_private_user_data,
                         bool can_kick,
                         bool can_replace_content,
                         bool can_rez,
                         bool can_rez_avatar_entities,
                         bool can_rez_tmp,
                         bool can_write_to_asset_server) {
        BotWsServer ws;
        QJsonObject jsonObj;
        QJsonObject dataObj {
            {"username", username},
            {"can_connect", can_connect},
            {"can_connect_past_max_capacity",can_connect_past_max_capacity},
            {"can_kick", can_kick},
            {"can_replace_content",can_replace_content},
            {"can_rez", can_rez},
            {"can_rez_avatar_entities", can_rez_avatar_entities},
            {"can_rez_tmp", can_rez_tmp},
            {"can_write_to_asset_server", can_write_to_asset_server}
        };

        jsonObj["event"] = "add_user";
        jsonObj["data"] = dataObj;

        QJsonDocument doc(jsonObj);
        QString strJson(doc.toJson(QJsonDocument::Compact));

        ws.sendClientMessage(strJson.toStdString());
    }
