#include <QJsonObject>
#include "websocket/websocketserver.h"
#include "utils.h"

void BotUtils::addOvertePlayer(std::string revPlatform, std::string username, bool can_adjust_locks,
                               bool can_connect,
                               bool can_connect_past_max_capacity,
                               bool can_get_and_set_private_user_data,
                               bool can_kick,
                               bool can_replace_content,
                               bool can_rez,
                               bool can_rez_avatar_entities,
                               bool can_rez_tmp,
                               bool can_write_to_asset_server,
                               BotWsServer &ws) {

    QJsonObject jsonObj;
    QJsonObject dataObj {
        {"username", username.c_str()},
        {"can_connect", can_connect},
        {"can_connect_past_max_capacity",can_connect_past_max_capacity},
        {"can_kick", can_kick},
        {"can_replace_content",can_replace_content},
        {"can_rez", can_rez},
        {"can_rez_avatar_entities", can_rez_avatar_entities},
        {"can_rez_tmp", can_rez_tmp},
        {"can_write_to_asset_server", can_write_to_asset_server}
    };

    jsonObj["platform"] = revPlatform.c_str();
    jsonObj["data"] = dataObj;
    jsonObj["event"] = "add_user";


    QJsonDocument doc(jsonObj);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    ws.sendClientMessage(strJson.toStdString());
}

void BotUtils::setOvertePlayerPermissions(std::string revPlatform, std::string username, bool can_adjust_locks,
                               bool can_connect,
                               bool can_connect_past_max_capacity,
                               bool can_get_and_set_private_user_data,
                               bool can_kick,
                               bool can_replace_content,
                               bool can_rez,
                               bool can_rez_avatar_entities,
                               bool can_rez_tmp,
                               bool can_write_to_asset_server,
                               BotWsServer &ws) {

    QJsonObject jsonObj;
    QJsonObject dataObj {
        {"username", username.c_str()},
        {"can_connect", can_connect},
        {"can_connect_past_max_capacity",can_connect_past_max_capacity},
        {"can_kick", can_kick},
        {"can_replace_content",can_replace_content},
        {"can_rez", can_rez},
        {"can_rez_avatar_entities", can_rez_avatar_entities},
        {"can_rez_tmp", can_rez_tmp},
        {"can_write_to_asset_server", can_write_to_asset_server}
    };

    jsonObj["platform"] = revPlatform.c_str();
    jsonObj["data"] = dataObj;
    jsonObj["event"] = "update_user";


    QJsonDocument doc(jsonObj);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    ws.sendClientMessage(strJson.toStdString());
}

void BotUtils::removeOvertePlayer(std::string revPlatform, std::string username, BotWsServer &ws) {

    QJsonObject jsonObj;
    QJsonObject dataObj {
        {"username", username.c_str()},
    };

    jsonObj["event"] = "remove_user";
    jsonObj["data"] = dataObj;

    QJsonDocument doc(jsonObj);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    ws.sendClientMessage(strJson.toStdString());
}

void BotUtils::banOvertePlayer(std::string revPlatform, std::string username,BotWsServer &ws) {

    QJsonObject jsonObj;
    QJsonObject dataObj {
        {"username", username.c_str()},
    };

    jsonObj["platform"] = revPlatform.c_str();
    jsonObj["event"] = "ban_user";
    jsonObj["data"] = dataObj;

    QJsonDocument doc(jsonObj);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    ws.sendClientMessage(strJson.toStdString());
}

void BotUtils::kickOvertePlayer(std::string revPlatform, std::string username,BotWsServer &ws) {

    QJsonObject jsonObj;
    QJsonObject dataObj {
        {"username", username.c_str()},
    };

    jsonObj["platform"] = revPlatform.c_str();
    jsonObj["event"] = "kick_user";
    jsonObj["data"] = dataObj;

    QJsonDocument doc(jsonObj);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    ws.sendClientMessage(strJson.toStdString());
}

void BotUtils::restartDomainServer(std::string revPlatform, BotWsServer &ws) {

    QJsonObject jsonObj;

    jsonObj["platform"] = revPlatform.c_str();
    jsonObj["event"] = "restart_server";
    jsonObj["data"] = {};

    QJsonDocument doc(jsonObj);
    QString strJson(doc.toJson(QJsonDocument::Compact));

    ws.sendClientMessage(strJson.toStdString());
}

void BotUtils::checkConfigFile() {
    std::ifstream config("Config.json");
    if (!config.good() | !config.exists() | !config.valid()) {
        std::ofstream newConfig("Config.json");

        newConfig << "{
    \"websocket_port\": 1337,
    \"discord\": {
        \"token\": \"\",
        \"welcome_channel\": \"\",
        \"preifx\": \"\"
    },
    \"matrix\": {
        \"username\": \"\",
        \"password\": \"\",
        \"preifx\": \"\"
    },
    \"commands\": {}
}";
        newConfig.close();
        std::cout << "Please fill out the config.json file and restart the bot." << std::endl;
        exit(0);
    }
}


void BotUtils::registerCommandsFromConfigFile(){
    
    std::filesystem::path currentDirectory = std::filesystem::current_path();
    std::ifstream config(currentDirectory + "/Config.json");
    if (!config.good() | !config.exists()) {
        std::cout << "Config file not found." << std::endl;
        exit(0);
    }

    QJsonDocument doc = QJsonDocument::fromJson(config.readAll());
    QJsonObject obj = doc.object();

    QJsonObject commandsObj = obj["commands"].toObject();

    for (auto cmd = commandsObj.begin(); cmd != commandsObj.end(); ++cmd) {
        //TODO: register commands here for all bots
    }
}
