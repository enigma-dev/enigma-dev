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

#ifndef ENIGMA_STEAMWORKS_API_MOCK
GCStatsAndAchievements::GCStatsAndAchievements()
    : m_CallbackUserStatsReceived(this, &GCStatsAndAchievements::on_user_stats_received),
      m_CallbackUserStatsStored(this, &GCStatsAndAchievements::on_user_stats_stored),
      m_CallbackAchievementStored(this, &GCStatsAndAchievements::on_achievement_stored),
      game_id_(SteamUtils()->GetAppID()),
      stats_valid_(false),
      steam_user_(nullptr),
      steam_user_stats_(nullptr) {
  GCStatsAndAchievements::steam_user_ = SteamUser();
  GCStatsAndAchievements::steam_user_stats_ = SteamUserStats();

  GCStatsAndAchievements::request_current_stats();
}
#else
GCStatsAndAchievements::GCStatsAndAchievements()
    : game_id_(SteamUtils()->GetAppID()), stats_valid_(false), steam_user_(nullptr), steam_user_stats_(nullptr) {
  GCStatsAndAchievements::steam_user_ = SteamUser();
  GCStatsAndAchievements::steam_user_stats_ = SteamUserStats();

  GCStatsAndAchievements::request_current_stats();
}
#endif  // ENIGMA_STEAMWORKS_API_MOCK

bool GCStatsAndAchievements::stats_valid() { return GCStatsAndAchievements::stats_valid_; }

bool GCStatsAndAchievements::set_achievement(const std::string& achievement_name) {
  return GCStatsAndAchievements::steam_user_stats_->SetAchievement(achievement_name.c_str());
}

bool GCStatsAndAchievements::get_achievement(const std::string& achievement_name, bool& achieved) {
  return GCStatsAndAchievements::steam_user_stats_->GetAchievement(achievement_name.c_str(), &achieved);
}

bool GCStatsAndAchievements::clear_achievement(const std::string& achievement_name) {
  return GCStatsAndAchievements::steam_user_stats_->ClearAchievement(achievement_name.c_str());
}

bool GCStatsAndAchievements::set_stat_int(const std::string& stat_name, const int32& value) {
  return GCStatsAndAchievements::steam_user_stats_->SetStat(stat_name.c_str(), value);
}

bool GCStatsAndAchievements::get_stat_int(const std::string& stat_name, int32& value) {
  return GCStatsAndAchievements::steam_user_stats_->GetStat(stat_name.c_str(), &value);
}

bool GCStatsAndAchievements::set_stat_float(const std::string& stat_name, const float& value) {
  return GCStatsAndAchievements::steam_user_stats_->SetStat(stat_name.c_str(), value);
}

bool GCStatsAndAchievements::get_stat_float(const std::string& stat_name, float& value) {
  return GCStatsAndAchievements::steam_user_stats_->GetStat(stat_name.c_str(), &value);
}

bool GCStatsAndAchievements::set_stat_average_rate(const std::string& stat_name, const float& count_this_session,
                                                   const double& session_length) {
  return GCStatsAndAchievements::steam_user_stats_->UpdateAvgRateStat(stat_name.c_str(), count_this_session,
                                                                      session_length);
}

// float gc_statsandachievements::get_stat_average_rate(const std::string& stat_name) {
//   return gc_statsandachievements::get_stat_float(stat_name);
// }

void GCStatsAndAchievements::on_user_stats_received(UserStatsReceived_t* pCallback) {
#ifndef ENIGMA_STEAMWORKS_API_MOCK
  if (GCStatsAndAchievements::game_id_.ToUint64() != pCallback->m_nGameID) return;
#endif  // ENIGMA_STEAMWORKS_API_MOCK

  if (GCStatsAndAchievements::stats_valid_) return;

#ifndef ENIGMA_STEAMWORKS_API_MOCK
  if (k_EResultOK == pCallback->m_eResult) {
    DEBUG_MESSAGE("Calling RequestCurrentStats succeeded.", M_INFO);

    GCStatsAndAchievements::stats_valid_ = true;

  } else {
    DEBUG_MESSAGE(
        "Calling RequestCurrentStats failed. Only returns false if there is no user logged in; otherwise, true." +
            std::to_string(pCallback->m_eResult),
        M_INFO);
  }
#else
  DEBUG_MESSAGE("Calling RequestCurrentStats succeeded.", M_INFO);

  GCStatsAndAchievements::stats_valid_ = true;
#endif  // ENIGMA_STEAMWORKS_API_MOCK
}

void GCStatsAndAchievements::on_user_stats_stored(UserStatsStored_t* pCallback) {
#ifndef ENIGMA_STEAMWORKS_API_MOCK
  if (GCStatsAndAchievements::game_id_.ToUint64() != pCallback->m_nGameID) return;
#endif  // ENIGMA_STEAMWORKS_API_MOCK

#ifndef ENIGMA_STEAMWORKS_API_MOCK
  if (k_EResultOK == pCallback->m_eResult) {
    DEBUG_MESSAGE("Calling StoreStats succeeded.", M_INFO);
  } else if (k_EResultInvalidParam == pCallback->m_eResult) {
    DEBUG_MESSAGE("Calling StoreStats with some stats that are failed to validate. Retrying ...", M_INFO);
    UserStatsReceived_t callback;
    callback.m_eResult = k_EResultOK;
    callback.m_nGameID = GCStatsAndAchievements::game_id_.ToUint64();
    on_user_stats_received(&callback);
    //gc_statsandachievements::store_stats();
  } else {
    DEBUG_MESSAGE("Calling StoreStats failed. Retrying ..." + std::to_string(pCallback->m_eResult), M_INFO);
    //gc_statsandachievements::store_stats();
  }
#else
  DEBUG_MESSAGE("Calling StoreStats succeeded.", M_INFO);
#endif  // ENIGMA_STEAMWORKS_API_MOCK
}

void GCStatsAndAchievements::on_achievement_stored(UserAchievementStored_t* pCallback) {
#ifndef ENIGMA_STEAMWORKS_API_MOCK
  if (GCStatsAndAchievements::game_id_.ToUint64() != pCallback->m_nGameID) return;
#endif  // ENIGMA_STEAMWORKS_API_MOCK

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
#ifndef ENIGMA_STEAMWORKS_API_MOCK
  return GCStatsAndAchievements::steam_user_stats_->StoreStats();
#else
  bool success = GCStatsAndAchievements::steam_user_stats_->StoreStats();
  {
    UserStatsStoredMock pCallback;
    pCallback.m_nGameID = INVALID_GAMEID;

    GCStatsAndAchievements::on_user_stats_stored(&pCallback);
  }

  {
    UserAchievementStoredMock pCallback;
    pCallback.m_nGameID = INVALID_GAMEID;
    pCallback.m_bGroupAchievement = false;
    pCallback.m_nCurProgress = 0;
    pCallback.m_nMaxProgress = 0;

    GCStatsAndAchievements::on_achievement_stored(&pCallback);
  }

  return success;
#endif  // ENIGMA_STEAMWORKS_API_MOCK
}

bool GCStatsAndAchievements::reset_all_stats() {
  return GCStatsAndAchievements::steam_user_stats_->ResetAllStats(false);
}

bool GCStatsAndAchievements::reset_all_stats_achievements() {
  return GCStatsAndAchievements::steam_user_stats_->ResetAllStats(true);
}

bool GCStatsAndAchievements::request_current_stats() {
#ifndef ENIGMA_STEAMWORKS_API_MOCK
  return GCStatsAndAchievements::steam_user_stats_->RequestCurrentStats();
#else
  bool success = GCStatsAndAchievements::steam_user_stats_->RequestCurrentStats();

  UserStatsReceived_t pCallback;
  pCallback.m_nGameID = INVALID_GAMEID;
  pCallback.m_steamIDUser = INVALID_STEAMID;

  GCStatsAndAchievements::on_user_stats_received(&pCallback);

  return success;
#endif  // ENIGMA_STEAMWORKS_API_MOCK
}

}  // namespace steamworks_gc
