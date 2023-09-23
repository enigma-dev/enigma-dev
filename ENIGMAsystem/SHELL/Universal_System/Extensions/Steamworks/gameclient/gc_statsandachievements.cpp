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

// m_CallbackUserStatsReceived(this, &GCStatsAndAchievements::on_user_stats_received),
// m_CallbackUserStatsStored(this, &GCStatsAndAchievements::on_user_stats_stored),
// m_CallbackAchievementStored(this, &GCStatsAndAchievements::on_achievement_stored),
GCStatsAndAchievements::GCStatsAndAchievements(uint32 game_id)
    : game_id_(game_id), stats_valid_(false), steam_user_(nullptr), steam_user_stats_(nullptr) {
  if (steamworks_b::Binder::SteamUser_v023 == nullptr || steamworks_b::Binder::SteamUserStats_v012 == nullptr) {
    DEBUG_MESSAGE("GCStatsAndAchievements::GCStatsAndAchievements() failed due to loading error.", M_ERROR);
    return;
  }

  GCStatsAndAchievements::steam_user_ = (ISteamUser*)steamworks_b::Binder::SteamUser_v023();
  GCStatsAndAchievements::steam_user_stats_ = (ISteamUserStats*)steamworks_b::Binder::SteamUserStats_v012();

  GCStatsAndAchievements::request_current_stats();
}

bool GCStatsAndAchievements::stats_valid() { return GCStatsAndAchievements::stats_valid_; }

bool GCStatsAndAchievements::set_achievement(const std::string& achievement_name) {
  if (steamworks_b::Binder::ISteamUserStats_SetAchievement == nullptr ||
      GCStatsAndAchievements::steam_user_stats_ == nullptr) {
    DEBUG_MESSAGE("GCStatsAndAchievements::set_achievement() failed due to loading error.", M_ERROR);
    return false;
  }

  return steamworks_b::Binder::ISteamUserStats_SetAchievement((void*)GCStatsAndAchievements::steam_user_stats_,
                                                              achievement_name.c_str());
}

bool GCStatsAndAchievements::get_achievement(const std::string& achievement_name, bool& achieved) {
  if (steamworks_b::Binder::ISteamUserStats_GetAchievement == nullptr ||
      GCStatsAndAchievements::steam_user_stats_ == nullptr) {
    DEBUG_MESSAGE("GCStatsAndAchievements::get_achievement() failed due to loading error.", M_ERROR);
    return false;
  }

  return steamworks_b::Binder::ISteamUserStats_GetAchievement((void*)GCStatsAndAchievements::steam_user_stats_,
                                                              achievement_name.c_str(), &achieved);
}

bool GCStatsAndAchievements::clear_achievement(const std::string& achievement_name) {
  if (steamworks_b::Binder::ISteamUserStats_ClearAchievement == nullptr ||
      GCStatsAndAchievements::steam_user_stats_ == nullptr) {
    DEBUG_MESSAGE("GCStatsAndAchievements::clear_achievement() failed due to loading error.", M_ERROR);
    return false;
  }

  return steamworks_b::Binder::ISteamUserStats_ClearAchievement((void*)GCStatsAndAchievements::steam_user_stats_,
                                                                achievement_name.c_str());
}

bool GCStatsAndAchievements::set_stat_int(const std::string& stat_name, const int32& value) {
  if (steamworks_b::Binder::ISteamUserStats_SetStatInt32 == nullptr ||
      GCStatsAndAchievements::steam_user_stats_ == nullptr) {
    DEBUG_MESSAGE("GCStatsAndAchievements::set_stat_int() failed due to loading error.", M_ERROR);
    return false;
  }

  return steamworks_b::Binder::ISteamUserStats_SetStatInt32((void*)GCStatsAndAchievements::steam_user_stats_,
                                                            stat_name.c_str(), value);
}

bool GCStatsAndAchievements::get_stat_int(const std::string& stat_name, int32& value) {
  if (steamworks_b::Binder::ISteamUserStats_GetStatInt32 == nullptr ||
      GCStatsAndAchievements::steam_user_stats_ == nullptr) {
    DEBUG_MESSAGE("GCStatsAndAchievements::get_stat_int() failed due to loading error.", M_ERROR);
    return false;
  }

  return steamworks_b::Binder::ISteamUserStats_GetStatInt32((void*)GCStatsAndAchievements::steam_user_stats_,
                                                            stat_name.c_str(), &value);
}

bool GCStatsAndAchievements::set_stat_float(const std::string& stat_name, const float& value) {
  if (steamworks_b::Binder::ISteamUserStats_SetStatFloat == nullptr ||
      GCStatsAndAchievements::steam_user_stats_ == nullptr) {
    DEBUG_MESSAGE("GCStatsAndAchievements::set_stat_float() failed due to loading error.", M_ERROR);
    return false;
  }

  return steamworks_b::Binder::ISteamUserStats_SetStatFloat((void*)GCStatsAndAchievements::steam_user_stats_,
                                                            stat_name.c_str(), value);
}

bool GCStatsAndAchievements::get_stat_float(const std::string& stat_name, float& value) {
  if (steamworks_b::Binder::ISteamUserStats_GetStatFloat == nullptr ||
      GCStatsAndAchievements::steam_user_stats_ == nullptr) {
    DEBUG_MESSAGE("GCStatsAndAchievements::get_stat_float() failed due to loading error.", M_ERROR);
    return false;
  }

  return steamworks_b::Binder::ISteamUserStats_GetStatFloat((void*)GCStatsAndAchievements::steam_user_stats_,
                                                            stat_name.c_str(), &value);
}

bool GCStatsAndAchievements::set_stat_average_rate(const std::string& stat_name, const float& count_this_session,
                                                   const double& session_length) {
  if (steamworks_b::Binder::ISteamUserStats_UpdateAvgRateStat == nullptr ||
      GCStatsAndAchievements::steam_user_stats_ == nullptr) {
    DEBUG_MESSAGE("GCStatsAndAchievements::set_stat_average_rate() failed due to loading error.", M_ERROR);
    return false;
  }

  return steamworks_b::Binder::ISteamUserStats_UpdateAvgRateStat((void*)GCStatsAndAchievements::steam_user_stats_,
                                                                 stat_name.c_str(), count_this_session, session_length);
}

// float gc_statsandachievements::get_stat_average_rate(const std::string& stat_name) {
//   return gc_statsandachievements::get_stat_float(stat_name);
// }

// void GCStatsAndAchievements::on_user_stats_received(UserStatsReceived_t* pCallback) {
//   if (GCStatsAndAchievements::game_id_.ToUint64() != pCallback->m_nGameID) return;

//   if (GCStatsAndAchievements::stats_valid_) return;

//   if (k_EResultOK == pCallback->m_eResult) {
//     DEBUG_MESSAGE("Calling RequestCurrentStats succeeded.", M_INFO);

//     GCStatsAndAchievements::stats_valid_ = true;

//   } else {
//     DEBUG_MESSAGE(
//         "Calling RequestCurrentStats failed. Only returns false if there is no user logged in; otherwise, true." +
//             std::to_string(pCallback->m_eResult),
//         M_INFO);
//   }
// }

// void GCStatsAndAchievements::on_user_stats_stored(UserStatsStored_t* pCallback) {
//   if (GCStatsAndAchievements::game_id_.ToUint64() != pCallback->m_nGameID) return;

//   if (k_EResultOK == pCallback->m_eResult) {
//     DEBUG_MESSAGE("Calling StoreStats succeeded.", M_INFO);
//   } else if (k_EResultInvalidParam == pCallback->m_eResult) {
//     DEBUG_MESSAGE("Calling StoreStats with some stats that are failed to validate. Retrying ...", M_INFO);
//     UserStatsReceived_t callback;
//     callback.m_eResult = k_EResultOK;
//     callback.m_nGameID = GCStatsAndAchievements::game_id_.ToUint64();
//     on_user_stats_received(&callback);
//     //gc_statsandachievements::store_stats();
//   } else {
//     DEBUG_MESSAGE("Calling StoreStats failed. Retrying ..." + std::to_string(pCallback->m_eResult), M_INFO);
//     //gc_statsandachievements::store_stats();
//   }
// }

// void GCStatsAndAchievements::on_achievement_stored(UserAchievementStored_t* pCallback) {
//   if (GCStatsAndAchievements::game_id_.ToUint64() != pCallback->m_nGameID) return;

//   if (0 == pCallback->m_nMaxProgress) {
//     DEBUG_MESSAGE("Achievement '" + std::string(pCallback->m_rgchAchievementName) + "' unlocked!.", M_INFO);
//   } else {
//     DEBUG_MESSAGE("Achievement '" + std::string(pCallback->m_rgchAchievementName) + "' progress callback, (" +
//                       std::to_string(pCallback->m_nCurProgress) + "," + std::to_string(pCallback->m_nMaxProgress) +
//                       ").",
//                   M_INFO);
//   }
// }

bool GCStatsAndAchievements::store_stats() {
  if (steamworks_b::Binder::ISteamUserStats_StoreStats == nullptr ||
      GCStatsAndAchievements::steam_user_stats_ == nullptr) {
    DEBUG_MESSAGE("GCStatsAndAchievements::store_stats() failed due to loading error.", M_ERROR);
    return false;
  }
  return steamworks_b::Binder::ISteamUserStats_StoreStats((void*)GCStatsAndAchievements::steam_user_stats_);
}

bool GCStatsAndAchievements::reset_all_stats() {
  if (steamworks_b::Binder::ISteamUserStats_ResetAllStats == nullptr ||
      GCStatsAndAchievements::steam_user_stats_ == nullptr) {
    DEBUG_MESSAGE("GCStatsAndAchievements::reset_all_stats() failed due to loading error.", M_ERROR);
    return false;
  }

  return steamworks_b::Binder::ISteamUserStats_ResetAllStats((void*)GCStatsAndAchievements::steam_user_stats_, false);
}

bool GCStatsAndAchievements::reset_all_stats_achievements() {
  if (steamworks_b::Binder::ISteamUserStats_ResetAllStats == nullptr ||
      GCStatsAndAchievements::steam_user_stats_ == nullptr) {
    DEBUG_MESSAGE("GCStatsAndAchievements::reset_all_stats_achievements() failed due to loading error.", M_ERROR);
    return false;
  }

  return steamworks_b::Binder::ISteamUserStats_ResetAllStats((void*)GCStatsAndAchievements::steam_user_stats_, true);
}

bool GCStatsAndAchievements::request_current_stats() {
  if (steamworks_b::Binder::ISteamUserStats_RequestCurrentStats == nullptr ||
      GCStatsAndAchievements::steam_user_stats_ == nullptr) {
    DEBUG_MESSAGE("GCStatsAndAchievements::request_current_stats() failed due to loading error.", M_ERROR);
    return false;
  }

#ifndef ENIGMA_STEAMWORKS_API_MOCK
  return steamworks_b::Binder::ISteamUserStats_RequestCurrentStats((void*)GCStatsAndAchievements::steam_user_stats_);
#else
  bool success =
      steamworks_b::Binder::ISteamUserStats_RequestCurrentStats((void*)GCStatsAndAchievements::steam_user_stats_);

  UserStatsReceived_t pCallback;
  pCallback.m_nGameID = INVALID_GAMEID;
  pCallback.m_steamIDUser = INVALID_STEAMID;

  GCStatsAndAchievements::on_user_stats_received(&pCallback);

  return success;
#endif  // ENIGMA_STEAMWORKS_API_MOCK
}

}  // namespace steamworks_gc
