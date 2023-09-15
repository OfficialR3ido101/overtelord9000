#include <QJsonObject>
#include <filesystem>
#include "../websocket/websocketserver.h"
#include "./utils.h"

using namespace std::filesystem;

void BotUtils::UserWizard()
{
    QFile config("Config.json");
    config.open(QIODevice::ReadOnly);
    QJsonObject obj = QJsonDocument::fromJson(config.readAll()).object();
    bool isComplete = obj["wizard_complete"].toBool();

    if (!isComplete)
    {
        qInfo() << "Welcome to the Overte Multi bot wizard!";
    }
    else
    {
        qInfo() << "You have already completed the wizard!";
        config.close();
        return;
    }
}

void BotUtils::registerCommandsFromConfigFile()
{

    std::filesystem::path current_directory = std::filesystem::current_path();
    std::filesystem::path file_path = current_directory / "Config.json";
    if (std::filesystem::exists(file_path))
    {
        std::cout << "Config file not found." << std::endl;
        return;
    }

    QFile config(file_path.c_str());

    QJsonDocument doc = QJsonDocument::fromJson(config.readAll());
    QJsonObject obj = doc.object();

    QJsonObject commandsObj = obj["commands"].toObject();

    for (auto cmd = commandsObj.begin(); cmd != commandsObj.end(); ++cmd)
    {
        // TODO: register commands here for all bots
    }
}

// check to see if Config.json exists in root directory
void BotUtils::checkConfigFileAndValidity()
{
    std::filesystem::path current_directory = std::filesystem::current_path();
    std::filesystem::path file_path = current_directory / "Config.json";
    QFile config(file_path.c_str());
    if (config.exists() && !config.nullptr)
    {
        qInfo() << "Config file found!";
        return;
    }
    else
    {
        qInfo() << "Config file not found!";
        return;
    }
    {
        std::cout << "Config file found." << std::endl;
        continue;
    }
    else if (!config.exists())
    {
        std::cout << "Config file not found." << std::endl;
        return;
    }

    // Check if all of the values are not null or nullptr
    bool all_values_not_null = true;
    if (obj["websocket_port"].isnull() ||
        obj["discord"]["token"].isnull() ||
        obj["discord"]["welcome_channel"].isnull() ||
        obj["matrix"]["username"].ismull() ||
        obj["matrix"]["password"].isnull() ||
        obj["matrix"]["prefix"].isnull() ||
        obj["commands"].isnull())
    {
        all_values_not_null = false;
    }

    // Check if the welcome channel is equal to an UNASSIGNED ULONG
    bool welcome_channel_is_string = config["discord"]["welcome_channel"].isstring().toulonglong();
    
    if (all_values_not_null)
    {
        std::cout << "All values are not null or nullptr." << std::endl;
        continue;
    }
    else
    {
        std::cout << "One or more values are null or nullptr." << std::endl;
        return;
    }

    if (welcome_channel_is_string)
    {
        std::cout << "The welcome channel is equal to an UNASSIGNED ULONG." << std::endl;
        continue;
    }
    else
    {
        std::cout << "The welcome channel is not equal to an UNASSIGNED ULONG." << std::endl;
        return;
    }
}
