/** Copyright (C) 2023-2024 Saif Kandil (k0T0z) (k0T0z)
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "gc_statsandachievements.h"

namespace steamworks_gc {

////////////////////////////////////////////////////////
// Public fields & functions
////////////////////////////////////////////////////////

GCStatsAndAchievements::GCStatsAndAchievements(uint32 app_id)
    : m_CallbackUserStatsReceived(this, &GCStatsAndAchievements::on_user_stats_received),
      m_CallbackUserStatsStored(this, &GCStatsAndAchievements::on_user_stats_stored),
      m_CallbackAchievementStored(this, &GCStatsAndAchievements::on_achievement_stored),
      game_id_(CGameID(app_id)),
      stats_valid_(false),
      steam_user_(nullptr),
      steam_user_stats_(nullptr) {
  GCStatsAndAchievements::steam_user_ = (ISteamUser*)steamworks_b::SteamBinder::SteamUser_vXXX();
  GCStatsAndAchievements::steam_user_stats_ = (ISteamUserStats*)steamworks_b::SteamBinder::SteamUserStats_vXXX();

  GCStatsAndAchievements::request_current_stats();
}

bool GCStatsAndAchievements::stats_valid() { return GCStatsAndAchievements::stats_valid_; }

bool GCStatsAndAchievements::set_achievement(const std::string& achievement_name) {
  return steamworks_b::SteamBinder::ISteamUserStats_SetAchievement((void*)GCStatsAndAchievements::steam_user_stats_,
                                                                   achievement_name.c_str());
}

bool GCStatsAndAchievements::get_achievement(const std::string& achievement_name, bool& achieved) {
  return steamworks_b::SteamBinder::ISteamUserStats_GetAchievement((void*)GCStatsAndAchievements::steam_user_stats_,
                                                                   achievement_name.c_str(), &achieved);
}

bool GCStatsAndAchievements::clear_achievement(const std::string& achievement_name) {
  return steamworks_b::SteamBinder::ISteamUserStats_ClearAchievement((void*)GCStatsAndAchievements::steam_user_stats_,
                                                                     achievement_name.c_str());
}

bool GCStatsAndAchievements::set_stat_int(const std::string& stat_name, const int32& value) {
  return steamworks_b::SteamBinder::ISteamUserStats_SetStatInt32((void*)GCStatsAndAchievements::steam_user_stats_,
                                                                 stat_name.c_str(), value);
}

bool GCStatsAndAchievements::get_stat_int(const std::string& stat_name, int32& value) {
  return steamworks_b::SteamBinder::ISteamUserStats_GetStatInt32((void*)GCStatsAndAchievements::steam_user_stats_,
                                                                 stat_name.c_str(), &value);
}

bool GCStatsAndAchievements::set_stat_float(const std::string& stat_name, const float& value) {
  return steamworks_b::SteamBinder::ISteamUserStats_SetStatFloat((void*)GCStatsAndAchievements::steam_user_stats_,
                                                                 stat_name.c_str(), value);
}

bool GCStatsAndAchievements::get_stat_float(const std::string& stat_name, float& value) {
  return steamworks_b::SteamBinder::ISteamUserStats_GetStatFloat((void*)GCStatsAndAchievements::steam_user_stats_,
                                                                 stat_name.c_str(), &value);
}

bool GCStatsAndAchievements::set_stat_average_rate(const std::string& stat_name, const float& count_this_session,
                                                   const double& session_length) {
  return steamworks_b::SteamBinder::ISteamUserStats_UpdateAvgRateStat(
      (void*)GCStatsAndAchievements::steam_user_stats_, stat_name.c_str(), count_this_session, session_length);
}

void GCStatsAndAchievements::on_user_stats_received(UserStatsReceived_t* pCallback) {
  if (GCStatsAndAchievements::game_id_.ToUint64() != pCallback->m_nGameID) return;

  if (GCStatsAndAchievements::stats_valid_) return;

  if (k_EResultOK == pCallback->m_eResult) {
    DEBUG_MESSAGE("Calling RequestCurrentStats succeeded.", M_INFO);

    GCStatsAndAchievements::stats_valid_ = true;

  } else {
    DEBUG_MESSAGE(
        "Calling RequestCurrentStats failed. Only returns false if there is no user logged in; otherwise, true." +
            std::to_string(pCallback->m_eResult),
        M_INFO);
  }
}

void GCStatsAndAchievements::on_user_stats_stored(UserStatsStored_t* pCallback) {
  if (GCStatsAndAchievements::game_id_.ToUint64() != pCallback->m_nGameID) return;

  if (k_EResultOK == pCallback->m_eResult) {
    DEBUG_MESSAGE("Calling StoreStats succeeded.", M_INFO);
  } else if (k_EResultInvalidParam == pCallback->m_eResult) {
    DEBUG_MESSAGE("Calling StoreStats with some stats that are failed to validate. Retrying ...", M_INFO);
    UserStatsReceived_t callback;
    callback.m_eResult = k_EResultOK;
    callback.m_nGameID = GCStatsAndAchievements::game_id_.ToUint64();
    on_user_stats_received(&callback);
    //GCStatsAndAchievements::store_stats();
  } else {
    DEBUG_MESSAGE("Calling StoreStats failed. Retrying ..." + std::to_string(pCallback->m_eResult), M_INFO);
    //GCStatsAndAchievements::store_stats();
  }
}

void GCStatsAndAchievements::on_achievement_stored(UserAchievementStored_t* pCallback) {
  if (GCStatsAndAchievements::game_id_.ToUint64() != pCallback->m_nGameID) return;

  if (0 == pCallback->m_nMaxProgress) {
    DEBUG_MESSAGE("Achievement '" + std::string(pCallback->m_rgchAchievementName) + "' unlocked!.", M_INFO);
  } else {
    DEBUG_MESSAGE("Achievement '" + std::string(pCallback->m_rgchAchievementName) + "' progress callback, (" +
                      std::to_string(pCallback->m_nCurProgress) + "," + std::to_string(pCallback->m_nMaxProgress) +
                      ").",
                  M_INFO);
  }
}

bool GCStatsAndAchievements::store_stats() {
  return steamworks_b::SteamBinder::ISteamUserStats_StoreStats((void*)GCStatsAndAchievements::steam_user_stats_);
}

bool GCStatsAndAchievements::reset_all_stats(const bool& achievements_too) {
  return steamworks_b::SteamBinder::ISteamUserStats_ResetAllStats((void*)GCStatsAndAchievements::steam_user_stats_,
                                                                  achievements_too);
}

bool GCStatsAndAchievements::request_current_stats() {
  return steamworks_b::SteamBinder::ISteamUserStats_RequestCurrentStats(
      (void*)GCStatsAndAchievements::steam_user_stats_);
}

}  // namespace steamworks_gc
