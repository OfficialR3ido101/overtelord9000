class webRequest
{
private:
    /* data */
public:
    void addOvertePlayer(std::string username, bool can_adjust_locks,
                         bool can_connect,
                         bool can_connect_past_max_capacity,
                         bool can_get_and_set_private_user_data,
                         bool can_kick,
                         bool can_replace_content,
                         bool can_rez,
                         bool can_rez_avatar_entities,
                         bool can_rez_tmp,
                         bool can_write_to_asset_server
                         );
    void setOvertePlayerPermissions(std::string username, bool can_adjust_locks,
                         bool can_connect,
                         bool can_connect_past_max_capacity,
                         bool can_get_and_set_private_user_data,
                         bool can_kick,
                         bool can_replace_content,
                         bool can_rez,
                         bool can_rez_avatar_entities,
                         bool can_rez_tmp,
                         bool can_write_to_asset_server
                         );
    void removeOvertePlayer(std::string username);
    void banOvertePlayer(std::string username);
    void kickOvertePlayer(std::string username);
    void restartDomainServer();
};
