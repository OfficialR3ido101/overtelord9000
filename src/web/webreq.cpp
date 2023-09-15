#include "webreq.h"
#include <filesystem>

using namespace std::filesystem;

void webRequest::addOvertePlayer(std::string username, bool can_adjust_locks,
                                 bool can_connect,
                                 bool can_connect_past_max_capacity,
                                 bool can_get_and_set_private_user_data,
                                 bool can_kick,
                                 bool can_replace_content,
                                 bool can_rez,
                                 bool can_rez_avatar_entities,
                                 bool can_rez_tmp,
                                 bool can_write_to_asset_server)
{
    // todo: add the code to send rest request to overte
}

void webRequest::setOvertePlayerPermissions(std::string username, bool can_adjust_locks,
                                            bool can_connect,
                                            bool can_connect_past_max_capacity,
                                            bool can_get_and_set_private_user_data,
                                            bool can_kick,
                                            bool can_replace_content,
                                            bool can_rez,
                                            bool can_rez_avatar_entities,
                                            bool can_rez_tmp,
                                            bool can_write_to_asset_server)
{

void webRequest::banOvertePlayer(std::string username)
{
    // todo: add the code to send rest request to overte
}

void webRequest::kickOvertePlayer(std::string username)
{
    //todo: add the code to send rest request to overte
}

void webRequest::removeOvertePlayer(std::string username)
{
    //todo: add the code to send rest request to overte
}

void webRequest::restartDomainServer()
{
    //todo: add the code to send rest request to overte
}