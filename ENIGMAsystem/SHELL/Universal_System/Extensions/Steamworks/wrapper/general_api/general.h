#if !defined(GENERAL_API_H)
#define GENERAL_API_H

#include <public\steam\steam_api.h>
// #include "../steam_exception.h"
#include <string>

class general {
private:
    static bool steam_initialised_;
    static bool steam_stats_ready_;
    static CSteamID steam_app_id_;
    static CSteamID steam_user_account_id_;
    static int64 steam_user_steam_id_;
    static std::string steam_persona_name_;

public:
    general() = default;
    ~general() = default;

    static bool steam_initialised();
    static bool steam_stats_ready();
    static CSteamID steam_get_app_id();
    static CSteamID steam_get_user_account_id();
    static int64 steam_get_user_steam_id();
    static std::string steam_get_persona_name();

};

#endif // !GENERAL_API_H
